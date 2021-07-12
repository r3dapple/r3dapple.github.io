#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include <fstream>

BigInteger modpow(BigInteger base, BigInteger exp, BigInteger mod);
BigInteger calculateJacobian(BigInteger a, BigInteger n);
BigInteger modulo(BigInteger base, BigInteger exponent, BigInteger mod);
bool solovoyStrassen(BigInteger p, int iterations);
BigInteger getLargePrimeQ(int keysize);
BigInteger getLargePrimeP(int keysize);

#endif // HELPER_H_INCLUDED
