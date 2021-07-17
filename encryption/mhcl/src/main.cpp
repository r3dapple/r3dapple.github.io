#include <iostream>
#include <string>
#include "biginteger/biginteger.hpp"
#include "rsa/rsa.hpp"
//#include "aes/aes.h"

int main(/*int argc, char** argv*/){

/*
	const unsigned char key[] = {0x41, 0xd, 0x5c, 0xcf, 0x7f, 0x93, 0x14, 0xbc, 0x35, 0x0, 0x6e, 0x0, 0x2c, 0xec, 0x0, 0x80, 0xd3, 0x65, 0x32, 0x10, 0xc3, 0xe5, 0x92, 0xf6, 0x19, 0x2c, 0x6d, 0x6c, 0x1f, 0xf, 0x49, 0x2f};
	const unsigned char initialization_vector[] = {0x2f, 0x7f, 0xf8, 0x3a, 0xd5, 0x2c, 0x11, 0x60, 0x8d, 0x98, 0xf5, 0x1d, 0xcf, 0xea, 0x3f, 0x95};
	
	aes machine(key, initialization_vector, 256, 2);
	machine.exportToCLI();
	machine.encrypt_file("testfile.txt", "testfile.txt.enc");
	machine.decrypt_file("testfile.txt.enc", "testfile_dec.txt");
*/

	rsa_keypair machine(128, 3, "TESTKEYS");
	std::string s = "123412545675647";
	BigInteger u(s);
	
	u.print();
	
	BigInteger enc = machine.encrypt(u);
	enc.print();
	
	BigInteger dec = machine.decrypt(enc);
	dec.print();

	return 0;
}
