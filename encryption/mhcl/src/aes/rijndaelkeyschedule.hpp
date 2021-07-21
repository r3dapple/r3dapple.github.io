#ifndef rijndaelkeyschedule_h
#define rijndaelkeyschedule_h

#include <iostream>
#include <cstring>
#include "galoisfield.hpp"
#include "termicolor.hpp"

class RijndaelKeySchedule{

	public:

		RijndaelKeySchedule(const unsigned char*, int, GaloisField*);
		~RijndaelKeySchedule();
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
		GaloisField* gfield;
		int expanded_key_length = 0;
		termicolor termcol;

};

#endif
