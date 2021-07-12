#include <iostream>
#include <string>
#include "mhcl/rsa/rsa.hh"

int main(int argc, char** argv){

	rsa_keypair machine(512, 65537, "TESTKEYS");
	std::string s = "123412545675647";
	BigUnsigned u = stringToBigUnsigned(s);
	
	std::cout << bigUnsignedToString(u) << std::endl;
	
	BigUnsigned enc = machine.encrypt(u);
	std::cout << bigUnsignedToString(enc) << std::endl;
	
	BigUnsigned dec = machine.decrypt(enc);
	std::cout << bigUnsignedToString(dec) << std::endl;
	
	return 0;
}
