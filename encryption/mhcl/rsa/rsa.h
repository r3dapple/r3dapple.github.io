#ifndef RSA_H_INCLUDED
#define RSA_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <cmath>
#include "../biginteger/biginteger.h"
#include "helper.hh"

class rsa_keypair {

private:
	int keySize;
	int e;
	const char* name;
	BigInteger p;
	BigInteger q;
	BigInteger n;
	BigInteger phi;
	BigInteger d;
	
public:
	rsa_keypair();
	rsa_keypair(std::string);
	rsa_keypair(int, int, const char*);

	void initializePublic(std::string);
	void save();
	void load(std::string);
	void loadPrivateFromString(std::string);
	BigInteger encrypt(BigInteger);
	BigInteger decrypt(BigInteger);
};

#endif // RSA_H_INCLUDED
