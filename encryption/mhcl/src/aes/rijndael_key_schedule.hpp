#ifndef rijndael_key_schedule_h
#define rijndael_key_schedule_h

#include <iostream>
#include <cstring>
#include "galois_field.h"
#include "termicolor.h"

class rijndael_key_schedule{

	public:

		rijndael_key_schedule(const unsigned char*, int, galois_field*);
		~rijndael_key_schedule();
		unsigned char* getExpandedKey();
		void printExpandedKey();
		unsigned char sbox(unsigned char);
		int getExpandedKeySize();

	private:

		void expand_key(const unsigned char*, int);
		void expand_key_128();
		void expand_key_192();
		void expand_key_256();

		void schedule_core(unsigned char*, unsigned char);
		
		void rotate(unsigned char*);
		unsigned char rcon(unsigned char);

		int key_size = 0;
		unsigned char* expanded_key = {0};
		galois_field* gfield;
		int expanded_key_length = 0;
		termicolor termcol;

};

#endif
