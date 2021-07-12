#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

BigUnsigned modpow(BigUnsigned base, BigUnsigned exp, BigUnsigned mod);

BigInteger calculateJacobian(BigInteger a, BigInteger n);

BigInteger modulo(BigInteger base, BigInteger exponent, BigInteger mod);

bool solovoyStrassen(BigInteger p, int iterations);

BigUnsigned getLargePrimeQ(int keysize);

BigUnsigned getLargePrimeP(int keysize);

#endif // HELPER_H_INCLUDED
