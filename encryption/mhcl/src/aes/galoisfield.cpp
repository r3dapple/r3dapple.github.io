#include "galoisfield.hpp"

GaloisField::GaloisField(){this->generate_tables(229);}
GaloisField::GaloisField(char generator){this->generate_tables(generator);}


unsigned char GaloisField::gadd(unsigned char a, unsigned char b) {return a ^ b;}
unsigned char GaloisField::gsub(unsigned char a, unsigned char b) {return a ^ b;}

unsigned char GaloisField::gmul_slow(unsigned char a, unsigned char b) {
	unsigned char p = 0;
	unsigned char counter;
	unsigned char hi_bit_set;
	for(counter = 0; counter < 8; counter++) {
		if((b & 1) == 1)
			p ^= a;
		hi_bit_set = (a & 0x80);
		a <<= 1;
		if(hi_bit_set == 0x80)
			a ^= 0x1b;
		b >>= 1;
	}
	return p;
}

void GaloisField::gmix_column(unsigned char *r) {
    unsigned char a[4];
    unsigned char b[4];
	unsigned char c;
	unsigned char h;	
	for(c=0;c<4;c++) {
		a[c] = r[c];
		h = r[c] & 0x80; /* hi bit */
		b[c] = r[c] << 1;
		if(h == 0x80)
			b[c] ^= 0x1b; /* Rijndael's Galois field */
	}
	r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
	r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
	r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
	r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];
}

void GaloisField::ginv_mix_column(unsigned char *r) {
        unsigned char a[4];
        unsigned char c;
        for(c=0;c<4;c++) {
			a[c] = r[c];
        }
        r[0] = gmul(a[0],14) ^ gmul(a[3],9) ^ gmul(a[2],13) ^ gmul(a[1],11);
        r[1] = gmul(a[1],14) ^ gmul(a[0],9) ^ gmul(a[3],13) ^ gmul(a[2],11);
        r[2] = gmul(a[2],14) ^ gmul(a[1],9) ^ gmul(a[0],13) ^ gmul(a[3],11);
        r[3] = gmul(a[3],14) ^ gmul(a[2],9) ^ gmul(a[1],13) ^ gmul(a[0],11);
}

void GaloisField::generate_tables(unsigned char generator){
	if(this->isGeneratorViable(generator) == false) return;

	this->atable[0] = 1;

	for(int i = 1; i < 256; i++){
		this->atable[i] = gmul_slow(atable[i-1], generator);
		int x = (atable[i] & 0x0f);
		int y = (atable[i] & 0xf0) >> 4;

		int rightside = i % 16;
		int leftside = i / 16;
		int value = (leftside << 4) ^ rightside;
		this->ltable[x + y * 0x10] = value;
	}

}

/* Calculate the s-box for a given number */
unsigned char GaloisField::sbox(unsigned char in) {
	unsigned char c, s, x;
	s = x = gmul_inverse(in);
	for(c = 0; c < 4; c++) {
		/* One bit circular rotate to the left */
		s = (s << 1) | (s >> 7);
		/* xor with x */
		x ^= s;
	}
	x ^= 99; /* 0x63 */
	return x;
}

unsigned char GaloisField::inv_sbox(unsigned char in) {
	unsigned char leftside = (in & 0xF0) >> 4;
	unsigned char rightside = (in & 0x0F);
	//std::cout << "Leftside: " << (int)leftside << " | " << "Rightside: " << (int)rightside << " | " << " Ergebnis: " << (int)leftside*17 + (int)rightside;
	return inv_sbtable[(int)leftside*16 + (int)rightside];
	//return inv_sbtable[0xf0];
}

unsigned char GaloisField::gmul(unsigned char a, unsigned char b) {
	int s;
	int q;
	int z = 0;
	s = ltable[a] + ltable[b];
	s %= 255;
	/* Get the antilog */
	s = atable[s];
	/* Now, we have some fancy code that returns 0 if either
           a or b are zero; we write the code this way so that the
           code will (hopefully) run at a constant speed in order to
           minimize the risk of timing attacks */
	q = s;
	if(a == 0) {
		s = z;
	}else {
		s = q;
	}

	if(b == 0) {
		s = z;
	}else {
		q = z;
	}

	return s;
}

unsigned char GaloisField::gmul_inverse(unsigned char in) {
	/* 0 is self inverting */
	if(in == 0) 
		return 0;
	else
        return atable[(255 - ltable[in])];
}

void GaloisField::printTables(){
	std::cout << std::hex;

	for(int i = 0; i < 256; i++){
		if(i % 16 == 0) std::cout << std::endl;
		std::cout << (int)atable[i] << " ";
	}
	
	std::cout << std::endl;
	std::cout << std::endl;

	for(int i = 0; i < 256; i++){
		if(i % 16 == 0) std::cout << std::endl;
		std::cout << (int)ltable[i] << " ";
	}

	std::cout << std::dec << std::endl;
	std::cout << std::endl;
}

bool GaloisField::isGeneratorViable(unsigned char generator){
	for(int i = 0; i < 128; i++){
		if(generator == possible_generators[i]){
			return true;
		}
	}

	std::cout << "Nonviable generator" << std::endl;
	return false;
}

void rotate(unsigned char *in) {
	unsigned char a,c;
	a = in[0];
	for(c=0;c<3;c++) 
		in[c] = in[c + 1];
	in[3] = a;
	return;
}
