#include <iostream>
#include <cstring>
#include <random>
#include "rijndael_key_schedule.h"
#include "galois_field.h"
#include <fstream>

class aes{

	public:
		aes(const unsigned char*, int, int);
		aes(const unsigned char*, const unsigned char*, int, int);
		aes(int, int);
		aes(){}
		void encrypt_file(const char*, const char*);
		void decrypt_file(const char*, const char*);

		unsigned char* encrypt_block(unsigned char*);
		unsigned char* decrypt_block(unsigned char*);

		unsigned char* getIV();
		void setIV(const unsigned char*);

		void exportToCLI();
		void exportToFile(const char*);
		void importFromFile(const char*, int);
		void importFromByteArray(char*, int);
		char* exportToByteArray(int&);

	private:
		void inv_shift_row_once(unsigned char*, int);
		void inv_shift_rows(unsigned char*);
		void shift_row_once(unsigned char*, int);
		void shift_rows(unsigned char*);
		void generatePSIV();
		char* readFileBytes(const char*, int&);
		void writeFileBytes(const char*, char*, int);
		const unsigned char* generateKey(int);

		rijndael_key_schedule* scheduler;
		galois_field* gfield;
		termicolor *termcol;
		unsigned char* expanded_key;
		int verbosity_level;	
		unsigned char* initialization_vector;
};
