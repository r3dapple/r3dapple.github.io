#include "rijndaelkeyschedule.hpp"

RijndaelKeySchedule::RijndaelKeySchedule(const unsigned char* key, int size, GaloisField* gfield){
	this->key_size = size;
	this->gfield = gfield;
	expand_key(key, size);
}

RijndaelKeySchedule::~RijndaelKeySchedule(){delete[] expanded_key;}

unsigned char* RijndaelKeySchedule::getExpandedKey(){return expanded_key;}
int RijndaelKeySchedule::getExpandedKeySize(){return key_size;}

void RijndaelKeySchedule::printExpandedKey(){
	if(expanded_key_length == 0) {std::cout << termcol.red() << "Cant print. Expanded Key Generation must have failed." << termcol.reset() << std::endl; return;}
	std::cout << std::hex << std::endl;
	std::cout <<  termcol.blue() << " [---]        Key Scheduling (" <<  termcol.orange() << "RKS" 
			<< termcol.blue() << ") initialised        [---]" << termcol.reset() << std::endl;
	std::cout << termcol.uwhite() << "Expanded Key:" << termcol.reset();

	for(int i = 0; i < expanded_key_length; i++){
		if(i % 16 == 0) std::cout << std::endl;
		std::cout << termcol.randomColor() <<(int)expanded_key[i] << termcol.reset() << " ";
	}
	std::cout << std::dec << std::endl;
	std::cout << std::endl;
}

void RijndaelKeySchedule::rotate(unsigned char *in) {
	unsigned char a, c;
	a = in[0];
	for(c=0;c<3;c++) 
		in[c] = in[c + 1];
	in[3] = a;
	return;
}

unsigned char RijndaelKeySchedule::rcon(unsigned char in) {
	unsigned char c = 1;
	if(in == 0)  
		return 0; 
	while(in != 1) {
		c = this->gfield->gmul(c, 2);
		in--;
	}
	return c;
}

/* This is the core key expansion, which, given a 4-byte value,
 * does some scrambling */
void RijndaelKeySchedule::schedule_core(unsigned char *in, unsigned char i) {
	char a;
	/* Rotate the input 8 bits to the left */
	rotate(in);
	/* Apply Rijndael's s-box on all 4 bytes */
	for(a = 0; a < 4; a++) 
		in[a] = gfield->sbox(in[a]);
	/* On just the first byte, add 2^i to the byte */
	in[0] ^= rcon(i);
}

void RijndaelKeySchedule::expand_key(const unsigned char* key, int bits){
	
	switch(bits){
		case 128:
			this->expanded_key_length = 176;
			this->expanded_key = new unsigned char[this->expanded_key_length];
			for(int i = 0; i < 16; i++){this->expanded_key[i] = key[i];}
			expand_key_128();
			break;
		case 192:
			this->expanded_key_length = 208;
			this->expanded_key = new unsigned char[this->expanded_key_length];
			for(int i = 0; i < 24; i++){this->expanded_key[i] = key[i];}
			expand_key_192();
			break;
		case 256:
			this->expanded_key_length = 240;
			this->expanded_key = new unsigned char[this->expanded_key_length];
			for(int i = 0; i < 32; i++){this->expanded_key[i] = key[i];}
			expand_key_256();
			break;
		default:
			std::cout << "Invalid key size" << std::endl;
	}
}

void RijndaelKeySchedule::expand_key_128() {
	unsigned char t[4];
	/* c is 16 because the first sub-key is the user-supplied key */
	unsigned char c = 16;
	unsigned char i = 1;
	unsigned char a;

	/* We need 11 sets of sixteen bytes each for 128-bit mode */
	while(c < 176) {
		/* Copy the temporary variable over from the last 4-byte
		* block */
		for(a = 0; a < 4; a++) 
			t[a] = this->expanded_key[a + c - 4];
		/* Every four blocks (of four bytes), 
		* do a complex calculation */
		if(c % 16 == 0) {
			schedule_core(t,i);
			i++;
		}
		for(a = 0; a < 4; a++) {
			this->expanded_key[c] = this->expanded_key[c - 16] ^ t[a];
			c++;
		}
	}
}

void RijndaelKeySchedule::expand_key_192() {
	unsigned char t[4];
	unsigned char c = 24;
	unsigned char i = 1;
	unsigned char a;
	while(c < 208) {
		/* Copy the temporary variable over */
		for(a = 0; a < 4; a++) 
			t[a] = this->expanded_key[a + c - 4]; 
		/* Every six sets, do a complex calculation */
		if(c % 24 == 0) {
			schedule_core(t,i);
			i++;
		}
		for(a = 0; a < 4; a++) {
			this->expanded_key[c] = this->expanded_key[c - 24] ^ t[a];
			c++;
		}
	}
}

void RijndaelKeySchedule::expand_key_256() {
	unsigned char t[4];
	unsigned char c = 32;
	unsigned char i = 1;
	unsigned char a;
	while(c < 240) {
		/* Copy the temporary variable over */
		for(a = 0; a < 4; a++)
			t[a] = this->expanded_key[a + c - 4]; 
		/* Every eight sets, do a complex calculation */
		if(c % 32 == 0) {
			schedule_core(t,i);
			i++;
		}
		/* For 256-bit keys, we add an extra gfield->sbox to the
		* calculation */
		if(c % 32 == 16) {
			for(a = 0; a < 4; a++) 
				t[a] = gfield->sbox(t[a]);
		}
		for(a = 0; a < 4; a++) {
			this->expanded_key[c] = this->expanded_key[c - 32] ^ t[a];
			c++;
		}
	}
}

