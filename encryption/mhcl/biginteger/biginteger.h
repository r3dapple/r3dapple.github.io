#ifndef BIGINTEGER_H_INCLUDED
#define BIGINTEGER_H_INCLUDED

#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>

class BigInteger{

	public:
		BigInteger(const char*);
		BigInteger(std::string);
		BigInteger(int);
		void print() const;
		void printBinary() const;
		void add(const BigInteger&);
		void multiply(const BigInteger&);
		void subtract(const BigInteger&);
		void divide(const BigInteger&);
		void mod(const BigInteger&);
		void pow(const BigInteger);
		void modpow(BigInteger, const BigInteger&);
		void bitAnd(const BigInteger&);
		void bitOr(const BigInteger&);
		void bitXOr(const BigInteger&);
		void bitNot(const BigInteger&);
		void bitShiftRight(int);
		void bitShiftLeft(int);
		bool operator <(const BigInteger&) const;
		bool operator >(const BigInteger&) const;
		bool operator ==(const BigInteger&) const;
		bool operator !=(const BigInteger&) const;
		bool operator <=(const BigInteger&) const;
		bool operator >=(const BigInteger&) const;
		BigInteger operator +(const BigInteger&) const;
		BigInteger operator -(const BigInteger&) const;
		BigInteger operator *(const BigInteger&) const;
		BigInteger operator /(const BigInteger&) const;
		BigInteger operator %(const BigInteger&) const;
		BigInteger operator ^(const BigInteger&) const;
		void operator +=(const BigInteger&);
		void operator -=(const BigInteger&);
		void operator *=(const BigInteger&);
		void operator /=(const BigInteger&);
		void operator %=(const BigInteger&);
		void operator ^=(const BigInteger&);
		char compare(const BigInteger&) const ;
		int getSize() const;
		char& operator [](int);
		char at(int) const;
		void resize(int);
		bool isNegative() const;
		void setNegative(bool);
		std::string getNumber() const;
		
	private:
		std::string number;
		bool isnegative;
		void add_digits(const BigInteger&);
		void subtract_digits(BigInteger);
		char compare_digits(const BigInteger&) const;
		BigInteger divide_digits(const BigInteger&, bool);
		std::string getBinaryString() const;
		void fromBinaryString(const std::string&);
		void invertNumber();
};

#endif // BIGINTEGER_H_INCLUDED
