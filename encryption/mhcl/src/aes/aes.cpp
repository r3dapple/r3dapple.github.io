#include "aes.hpp"

// https://kavaliro.com/wp-content/uploads/2014/03/AES.pdf

char* aes::readFileBytes(const char* filename, int& filesize){
	FILE *fileptr;
	char *buffer;

	fileptr = fopen(filename, "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filesize = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file

	buffer = (char *)malloc(filesize * sizeof(char)); // Enough memory for the file
	fread(buffer, filesize, 1, fileptr); // Read in the entire file
	fclose(fileptr); // Close the file
	return buffer;
}

void aes::writeFileBytes(const char* outFilename, char* byteArray, int byteArraySize){
	std::ofstream output (outFilename, std::ios::binary | std::ios::out);
	output.write(byteArray, byteArraySize);
	output.close();
}

void aes::encrypt_file(const char* filepath, const char* outpath){

	int filesize;
	char* fileByteArray = readFileBytes(filepath, filesize);
	
	// we need padding if file isnt splittable into 16 byte parts (PKCS#7)
	// https://en.wikipedia.org/wiki/PKCS_7
	int div16mod0 = (16 - (filesize % 16)); // how many pads

	char* paddedfileByteArray = new char[filesize + div16mod0];
	std::memcpy(paddedfileByteArray, fileByteArray, filesize);
	delete[] fileByteArray;
	for(int i = filesize; i < filesize + div16mod0; i++){
		paddedfileByteArray[i] = div16mod0;
	}
	filesize = filesize + div16mod0;

	for(int i = 0; i < filesize; i+=16){

		// CBC Implementation https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#CBC | DISCLAIMER: THIS DOES NOT IMPLEMENT AUTHENTICATION, USE EAX OR GCM FOR THAT
		// The IV (initialization vector) has to be unpredictable at encryption time
		for(int j = 0; j < 16; j++){
			if(i == 0)(paddedfileByteArray+i)[j] = (paddedfileByteArray+i)[j] ^ getIV()[j];
			else (paddedfileByteArray+i)[j] = (paddedfileByteArray+i)[j] ^ (paddedfileByteArray+i-16)[j];
		}
		unsigned char* cypher = encrypt_block((unsigned char*)paddedfileByteArray+i);
		std::memcpy(paddedfileByteArray+i, cypher, 16);
		delete[] cypher;
	}


	writeFileBytes(outpath, paddedfileByteArray, filesize);
}

void aes::decrypt_file(const char* filepath, const char* outpath){
	int filesize;
	char* paddedfileByteArray = readFileBytes(filepath, filesize);
	
	char* fileByteArray = new char[filesize];

	// decode
	for(int i = 0; i < filesize; i+=16){
		unsigned char* message = decrypt_block((unsigned char*)paddedfileByteArray+i);

		// CBC Implementation https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#CBC | DISCLAIMER: THIS DOES NOT IMPLEMENT AUTHENTICATION, USE EAX OR GCM FOR THAT
		// The IV (initialization vector) has to be unpredictable at encryption time
		for(int j = 0; j < 16; j++){
			if(i == 0) message[j] = message[j] ^ getIV()[j];
			else message[j] = message[j] ^ (paddedfileByteArray+i-16)[j];
		}

		std::memcpy(fileByteArray+i, message, 16);
		delete[] message;
	}

	// determine where padding begins and only copy content from before
	writeFileBytes(outpath, fileByteArray, filesize - fileByteArray[filesize-1]);
	delete[] paddedfileByteArray;
	delete[] fileByteArray;
}

void aes::generatePSIV(){
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<int> distribution(0,255);

	for(int i = 0; i < 16; i++){
		initialization_vector[i] = distribution(generator);
	}
}

const unsigned char* aes::generateKey(int key_size){
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<int> distribution(0,255);

	int length;
	if(key_size == 128) length = 16;
	else if(key_size == 192) length = 24;
	else if(key_size == 256) length = 32;

	unsigned char* key = new unsigned char[length];
	
	for(int i = 0; i < length; i++){
		key[i] = distribution(generator);
	}

	return (const unsigned char*)key;
}

aes::aes(int key_size=128, int verbosity_level=2){

	const unsigned char* key = generateKey(key_size);
	// hier liegt der key noch in memory (possible attack)

	this->gfield = new galois_field(192);
	if(verbosity_level == 3)gfield->printTables();

	scheduler = new rijndael_key_schedule(key, key_size, gfield);
	if(verbosity_level >= 1)scheduler->printExpandedKey();
	this->expanded_key = scheduler->getExpandedKey();
	
	this->verbosity_level = verbosity_level;

	initialization_vector = new unsigned char[16];
	generatePSIV();

	termcol = new termicolor;
}

aes::aes(const unsigned char* key, int key_size=128, int verbosity_level=2){
	this->gfield = new galois_field(192);
	if(verbosity_level == 3)gfield->printTables();

	scheduler = new rijndael_key_schedule(key, key_size, gfield);
	if(verbosity_level >= 1)scheduler->printExpandedKey();
	this->expanded_key = scheduler->getExpandedKey();
	
	this->verbosity_level = verbosity_level;

	initialization_vector = new unsigned char[16];
	generatePSIV();

	termcol = new termicolor;
}

aes::aes(const unsigned char* key, const unsigned char* IV, int key_size=128, int verbosity_level=2){
	this->gfield = new galois_field(192);
	if(verbosity_level == 3)gfield->printTables();

	scheduler = new rijndael_key_schedule(key, key_size, gfield);
	if(verbosity_level >= 1)scheduler->printExpandedKey();
	this->expanded_key = scheduler->getExpandedKey();
	
	this->verbosity_level = verbosity_level;

	initialization_vector = new unsigned char[16];
	setIV(IV);

	termcol = new termicolor;
}

void aes::exportToCLI(){

	int length = -1;
	if(scheduler->getExpandedKeySize() == 128) length = 16;
	else if(scheduler->getExpandedKeySize() == 192) length = 24;
	else if(scheduler->getExpandedKeySize() == 256) length = 32;
	
	if(verbosity_level >= 1){ 
		std::cout <<  termcol->green() << " [---]        CLI EXPORT (" <<  termcol->white() << "CLIEX" 
			<< termcol->green() << ")        [---]" << termcol->reset() << std::endl;
	}

	std::cout << "const unsigned char key[] = {";
	for(int i = 0; i < length-1; i++){
		std::cout << "0x" << std::hex << (int)scheduler->getExpandedKey()[i] << ", ";
	}
	std::cout << "0x" << std::hex << (int)scheduler->getExpandedKey()[length-1] << "};" << std::endl;
	
	std::cout << "const unsigned char initialization_vector[] = {";
	for(int i = 0; i < 15; i++){
		std::cout << "0x" << std::hex << (int)getIV()[i] << ", ";
	}
	std::cout << "0x" << std::hex << (int)getIV()[15] << "};" << std::endl;
}

void aes::exportToFile(const char* filepath){

	char length = -1;
	if(scheduler->getExpandedKeySize() == 128) length = 16;
	else if(scheduler->getExpandedKeySize() == 192) length = 24;
	else if(scheduler->getExpandedKeySize() == 256) length = 32;
	
	char* outByteArray = new char[1+(int)length+16];

	if(verbosity_level >= 1){ 
		std::cout <<  termcol->green() << " [---]        FILE EXPORT (" <<  termcol->white() << "FEX" 
			<< termcol->green() << ")        [---]" << termcol->reset() << std::endl;
		std::cout << filepath << std::endl;
	}

	outByteArray[0] = length;
	
	for(int i = 0; i < (int)length; i++){outByteArray[i+1] = scheduler->getExpandedKey()[i];}
	for(int i = 0; i < 16; i++){outByteArray[i+1+length] = getIV()[i];}

	writeFileBytes(filepath, outByteArray, 1+(int)length+16);
}

char* aes::exportToByteArray(int& arraysize){

	char length = -1;
	if(scheduler->getExpandedKeySize() == 128) length = 16;
	else if(scheduler->getExpandedKeySize() == 192) length = 24;
	else if(scheduler->getExpandedKeySize() == 256) length = 32;
	
	char* outByteArray = new char[1+(int)length+16];

	if(verbosity_level >= 1){ 
		std::cout <<  termcol->green() << " [---]        ARRAY EXPORT (" <<  termcol->white() << "ARRX" 
			<< termcol->green() << ")        [---]" << termcol->reset() << std::endl;
	}

	outByteArray[0] = length;
	
	for(int i = 0; i < (int)length; i++){outByteArray[i+1] = scheduler->getExpandedKey()[i];}
	for(int i = 0; i < 16; i++){outByteArray[i+1+length] = getIV()[i];}
	
	arraysize = 1 + (int)length + 16;

	return outByteArray;
}

void aes::importFromByteArray(char* inByteArray, int verbosity){

	char length = inByteArray[0];

	if(verbosity_level >= 1){ 
		std::cout <<  termcol->green() << " [---]        ARRAY IMPORT (" <<  termcol->white() << "ARRIM" 
			<< termcol->green() << ")        [---]" << termcol->reset() << std::endl;
	}
	
	unsigned char* iv = new unsigned char[16];
	unsigned char* key = new unsigned char[(int)length];

	for(int i = 0; i < (int)length; i++){key[i] = inByteArray[i+1];}
	for(int i = 0; i < 16; i++){iv[i] = inByteArray[i+1+(int)length];}

	int size = 0;
	if(length == 16) size = 128;
	else if(length == 24) size = 192;
	else if(length == 32) size = 256;

	// aes(const unsigned char*, const unsigned char*, int, int);
	aes temp_aes(key, iv, size, verbosity);

	*this = temp_aes;
}

void aes::importFromFile(const char* filepath, int verbosity){

	int filesize;
	char* fileByteArray = readFileBytes(filepath, filesize);

	if(verbosity_level >= 1){ 
		std::cout <<  termcol->green() << " [---]        FILE IMPORT (" <<  termcol->white() << "FIM" 
			<< termcol->green() << ")        [---]" << termcol->reset() << std::endl;
		std::cout << filepath << std::endl;
	}

	importFromByteArray(fileByteArray, verbosity);
}

unsigned char* aes::encrypt_block(unsigned char* message){

	if(verbosity_level >= 1){ 
		std::cout <<  termcol->green() << " [---]        ENCRYPTION (" <<  termcol->white() << "ENCR" 
			<< termcol->green() << ")        [---]" << termcol->reset() << std::endl;
	}

	// add round key
	unsigned char* cypher = new unsigned char[16];
	for(int i = 0; i < 16; i++){cypher[i] = gfield->gadd(expanded_key[i], message[i]);}

	if(verbosity_level >= 2) {
		
		std::cout << termcol->blue() << " [---]        Add 0th Round Key (" <<  termcol->orange() << "ARK0" 
				<< termcol->blue() << ")        [---]" << termcol->reset() << std::endl; std::cout << "Cypher: ";
		for(int i = 0; i < 16; i++){std::cout << std::hex << (int)cypher[i] << " ";}
		std::cout << std::endl;
		std::cout << std::endl;
	}




	// determine number of rounds
	int rounds;
	if(scheduler->getExpandedKeySize() == 128) rounds = 10;
	else if(scheduler->getExpandedKeySize() == 192) rounds = 12;
	else if(scheduler->getExpandedKeySize() == 256) rounds = 14;
	else rounds = -1;
	
	// start rounds
	for(int round = 1; round <= rounds; round++){
		if(verbosity_level >= 2){

			std::cout <<  termcol->red() << " [---]        Round Number (" <<  termcol->magenta() << std::dec << round
				<< termcol->red() << ")        [---]" << termcol->reset() << std::endl;
		}


		// substitue bytes
		if(verbosity_level >= 2){std::cout <<  termcol->blue() << " [---]        Substitute Bytes (" <<  termcol->orange() << "SB" 
				<< termcol->blue() << ")        [---]" << termcol->reset() << std::endl; std::cout << "Cypher: ";}

		for(int i = 0; i < 16; i++){
			cypher[i] = gfield->sbox(cypher[i]);
			if(verbosity_level >= 2) std::cout << std::hex << (int)cypher[i] << " ";
		}


		// shift rows
		shift_rows(cypher);
		if(verbosity_level >= 2){
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout <<  termcol->blue() << " [---]        Shift Rows (" <<  termcol->orange() << "SR" 
				<< termcol->blue() << ")        [---]" << termcol->reset() << std::endl;
			std::cout << "Cypher: ";
			if(verbosity_level >= 2) {for(int i = 0; i < 16; i++){std::cout << std::hex << (int)cypher[i] << " ";}}
		}
		

		// mix columns
		if(round < rounds){
			
			for(int i = 0; i < 13; i+=4){ gfield->gmix_column(cypher + i); }

			if(verbosity_level >= 2){
				std::cout << std::endl;
				std::cout << std::endl;
				std::cout <<  termcol->blue() << " [---]        Mix Columns (" <<  termcol->orange() << "MC" 
					<< termcol->blue() << ")        [---]" << termcol->reset() << std::endl;
				std::cout << "Cypher: ";
				for(int i = 0; i < 16; i++){std::cout << std::hex << (int)cypher[i] << " ";}
				
			}
		}

		// add round key
		for(int i = 0; i < 16; i++){cypher[i] = gfield->gadd(expanded_key[i+round*16], cypher[i]);}

		if(verbosity_level >= 2){
			std::cout << std::endl;
			std::cout << std::endl;
			std::cout <<  termcol->blue() << " [---]        Add " << round << ". Round Key (" <<  termcol->orange() << "ARK" << round 
					<< termcol->blue() << ")        [---]" << termcol->reset() << std::endl;
			std::cout << "Round Key: ";
			for(int i = 0; i < 16; i++){std::cout << (int)(unsigned char)expanded_key[i+round*16] << " ";}
			std::cout << std::endl;
			std::cout << "Cypher: ";
			for(int i = 0; i < 16; i++){std::cout << std::hex << (int)cypher[i] << " ";}
			std::cout << std::endl;
			std::cout << std::endl;
		}
	}

	if(verbosity_level >= 1){
		std::cout << "Cypher: ";
		for(int i = 0; i < 16; i++){std::cout << std::hex << (int)(unsigned char)cypher[i] << " ";}
		std::cout << std::endl;
		std::cout << std::endl;
	}

	return cypher;

}
unsigned char* aes::decrypt_block(unsigned char* in_cypher){
	// Decryption
	if(verbosity_level >= 1){
		std::cout <<  termcol->green() << " [---]        DECRYPTION (" <<  termcol->white() << "DECR" 
			<< termcol->green() << ")        [---]" << termcol->reset() << std::endl;
	}

	unsigned char* cypher = new unsigned char[16];
	std::memcpy(cypher, in_cypher, 16);

	// determine number of rounds
	int rounds;
	if(scheduler->getExpandedKeySize() == 128) rounds = 10;
	else if(scheduler->getExpandedKeySize() == 192) rounds = 12;
	else if(scheduler->getExpandedKeySize() == 256) rounds = 14;
	else rounds = -1;

	for(int round = rounds; round >= 1; round--){

		// sub round key
		for(int i = 0; i < 16; i++){cypher[i] = gfield->gsub(expanded_key[i+round*16], cypher[i]);}

		if(verbosity_level >= 2){

			std::cout <<  termcol->red() << " [---]        Inverse Round Number (" <<  termcol->magenta() << std::dec << round
				<< termcol->red() << ")        [---]" << termcol->reset() << std::endl;
			
			std::cout <<  termcol->blue() << " [---]        Subtract " << round << ". Round Key (" <<  termcol->orange() << "SRK" << round 
					<< termcol->blue() << ")        [---]" << termcol->reset() << std::endl;
			
			
			std::cout << "Round Key: ";
			for(int i = 0; i < 16; i++){std::cout << std::hex << (int)(unsigned char)expanded_key[i+round*16] << " ";}
			std::cout << std::endl;
			std::cout << "Cypher: ";
			for(int i = 0; i < 16; i++){std::cout << std::hex << (int)cypher[i] << " ";}
			std::cout << std::endl;
			std::cout << std::endl;

		}

		

		if(round < rounds){
			// inv mix columns
			for(int i = 0; i < 13; i+=4){ gfield->ginv_mix_column(cypher + i); }
	
			if(verbosity_level >= 2){
				std::cout <<  termcol->blue() << " [---]        Inverse Mix Columns (" <<  termcol->orange() << "IMC" 
					<< termcol->blue() << ")        [---]" << termcol->reset() << std::endl;
				
				std::cout << "Cypher: ";
				for(int i = 0; i < 16; i++){std::cout << std::hex << (int)cypher[i] << " ";}
				std::cout << std::endl;
				std::cout << std::endl;
			}
		}

		
		// inv shift rows
		inv_shift_rows(cypher);

		if(verbosity_level >= 2){
			std::cout <<  termcol->blue() << " [---]        Inverse Shift Rows (" <<  termcol->orange() << "ISR" 
				<< termcol->blue() << ")        [---]" << termcol->reset() << std::endl;
			
			std::cout << "Cypher: ";
			for(int i = 0; i < 16; i++){std::cout << std::hex << (int)cypher[i] << " ";}
			std::cout << std::endl;
			std::cout << std::endl;
		}

		// inv substitue bytes
		for(int i = 0; i < 16; i++){cypher[i] = gfield->inv_sbox(cypher[i]);}

		if(verbosity_level >= 2){
			std::cout <<  termcol->blue() << " [---]        Inverse Substitute Bytes (" <<  termcol->orange() << "ISB" 
					<< termcol->blue() << ")        [---]" << termcol->reset() << std::endl;
		
			std::cout << "Cypher: ";
			for(int i = 0; i < 16; i++){std::cout << std::hex << (int)cypher[i] << " ";}
			std::cout << std::endl;
			std::cout << std::endl;
		}
	
	}

	// subtract round key	
	for(int i = 0; i < 16; i++){cypher[i] = gfield->gsub(expanded_key[i], cypher[i]);}

	if(verbosity_level >= 2){
		std::cout <<  termcol->blue() << " [---]        Subtract 0th Round Key (" <<  termcol->orange() << "SRK0" 
				<< termcol->blue() << ")        [---]" << termcol->reset() << std::endl;

		std::cout << "Cypher: ";
		for(int i = 0; i < 16; i++){std::cout << std::hex << (int)cypher[i] << " ";}
		std::cout << std::endl;
		std::cout << std::endl;
	}
	
	if(verbosity_level >= 1){
		std::cout << "Message: ";
		for(int i = 0; i < 16; i++){std::cout << std::hex << (int)(unsigned char)cypher[i] << " ";}
		std::cout << std::endl;
		std::cout << std::endl;
	}

	return cypher;
}

unsigned char* aes::getIV(){return initialization_vector;}
void aes::setIV(const unsigned char* iv){std::memcpy(initialization_vector, iv, 16);}

void aes::inv_shift_row_once(unsigned char* cypher, int row){
	int x = cypher[row+4*3];

	cypher[row+4*3] = cypher[row+4*2];
	cypher[row+4*2] = cypher[row+4*1];
	cypher[row+4*1] = cypher[row];
	cypher[row] = x;
}

void aes::inv_shift_rows(unsigned char* cypher){
	for(int i = 1; i < 4; i++){
		for(int j = 0; j < i; j++){
			inv_shift_row_once(cypher, i);
		}
	}
}

void aes::shift_row_once(unsigned char* cypher, int row){
	int x = cypher[row];
	cypher[row] = cypher[row+4*1];
	cypher[row+4*1] = cypher[row+4*2];
	cypher[row+4*2] = cypher[row+4*3];
	cypher[row+4*3] = x;
}

void aes::shift_rows(unsigned char* cypher){
	for(int i = 1; i < 4; i++){
		for(int j = 0; j < i; j++){
			shift_row_once(cypher, i);
		}
	}
}

