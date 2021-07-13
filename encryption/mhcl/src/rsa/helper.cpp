#include "helper.h"

bool fileExists(const char *name) {
    std::ifstream f(name);
    return f.good();
}

// To calculate Jacobian symbol of a given number
BigInteger calculateJacobian(BigInteger a, BigInteger n){
    if (a == 0)
        return 0;// (0/n) = 0

    BigInteger ans = 1;

    if (a == 1)
        return ans;// (1/n) = 1

    while (a != 0){
        if (a < 0){
            a = a*-1;//(a/n) = (-a/n)*(-1/n)
            if (n % 4 == 3)
                ans = ans*(-1);// (-1/n) = -1 if n = 3 (mod 4)
        }

        while (a % 2 == 0){
            a = a / 2;
            if (n % 8 == 3 || n % 8 == 5)
                ans = ans*(-1);
        }

        std::swap(a, n);

        if (a % 4 == 3 && n % 4 == 3)
            ans = ans*(-1);
        a = a % n;

        if (a > n / 2)
            a = a - n;
    }

    if (n == 1)
        return ans;

    return 0;
}


BigInteger modulo(BigInteger base, BigInteger exponent, BigInteger mod){
    BigInteger x = 1;
    BigInteger y = base;
    while (exponent > 0){
        if (exponent % 2 == 1)
            x = (x * y) % mod;

        y = (y * y) % mod;
        exponent = exponent / 2;
    }

    return x % mod;
}


// To perform the Solovay-Strassen Primality Test
bool solovoyStrassen(BigInteger p, int iterations)
{
    if (p < 2)
        return false;
    if (p != 2 && p % 2 == 0)
        return false;

    for (int i = 0; i < iterations; i++)
    {
        // Generate a random number a
        BigInteger a = BigInteger(rand()) % (p - 1) + 1;
        BigInteger jacobian = (p + calculateJacobian(a, p)) % p;
        BigInteger mod = modulo(a, (p - 1) / 2, p);

        if (jacobian == 0 || mod != jacobian)
            return false;
    }
    return true;
}

BigInteger getLargePrimeQ(int keysize){
    // begins at 11111111111111111 ... keysize/2 1's
    BigInteger candidate = 1;
    BigInteger one = 1;
    for(int i = 0; i < ((keysize)/2)-1; i++){
        candidate.bitShiftLeft(1);
        candidate.bitOr(1);
    }
    std::cout << candidate << std::endl;

    for (;;) {
        if ( solovoyStrassen(candidate, 65) ){
            std::cout << candidate << " is prime" << std::endl;
            return candidate;
        }
        candidate += rand();
        candidate += rand();
    }
}

BigInteger getLargePrimeP(int keysize){
    // begins at 100000000000 ... keysize/2-1 0's and one 1
    BigInteger candidate = 1;
    candidate.bitShiftLeft(((keysize)/2)-1);
    std::cout << candidate << std::endl;

    for (;;) {
        if ( solovoyStrassen(candidate, 65) ){
            std::cout << candidate << " is prime" << std::endl;
            return candidate;
        }
        candidate += rand();
        candidate += rand();
    }
}
