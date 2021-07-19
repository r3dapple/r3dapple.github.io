#include "biginteger.hpp"

BigInteger::BigInteger(const char* n){
	int size = std::strlen(n);
	int lowerbound = 0;
	if(n[0] == '-'){
		isnegative = true;
		// dont read the '-' into the numberstring
		lowerbound = 1;
	}else{
		isnegative = false;
	}
	
	number.resize(size-lowerbound, 0);

	try{
		for(int i = size-1; i >= lowerbound; i--){
			// if we read at any point a non digit character, fall back on zero initialization
			if(not std::isdigit(n[i])) throw std::invalid_argument("BigInteger(const char* n): '" + std::to_string(n[i]) + "' Non-digit input. Falling back on zero initialisation.");
			number[size-1-i] = n[i] - '0';
		}
	}catch(std::invalid_argument &e){
		number.resize(1);
		number[0] = 0;
		std::cerr << e.what() << std::endl;
	}

	// special case: if number is zero, the number should always be positive
	if(getNumber(false) == "0") setNegative(false);
}

BigInteger::BigInteger(std::string s) : BigInteger(s.c_str()){}
BigInteger::BigInteger(int i) : BigInteger(std::to_string(i)){}
BigInteger::BigInteger(long l) : BigInteger(std::to_string(l)){}
BigInteger::BigInteger() : BigInteger("0"){}

void BigInteger::print() const {
	if(isnegative)std::cout << '-';
	
	for(int i = number.size()-1; i >= 0; i--){
		std::cout << (int)(number[i]);
	}
	std::cout << std::endl;
}

void BigInteger::printBinary() const {
	if(isnegative)std::cout << '-';
	std::string bin = getBinaryString();
	for(int i = bin.size()-1; i >= 0; i--){
		std::cout << (int)(bin[i]);
	}
	std::cout << std::endl;
}

void BigInteger::add(const BigInteger &summand2){
	if(isNegative() == summand2.isNegative()){
		add_digits(summand2);
	}else{
		subtract_digits(summand2);
	}
	// special case: if result is zero, the number should always be positive
	if(getNumber(false) == "0") setNegative(false);
}

// O(n*m)
void BigInteger::multiply(const BigInteger &multiplicator){
	BigInteger result("0");
	// if one of the multiplicators is negative but not both, the result is negative
	if((multiplicator.isNegative() || isnegative) && !(multiplicator.isNegative() && isnegative)) result.setNegative(true);
	
	int maxlengthofresult = (int)number.size() + multiplicator.getSize();
	result.resize(maxlengthofresult);
	
	for(std::size_t i = 0; i < multiplicator.getSize(); i++){
		char digit1 = multiplicator.at(i);
		char carry = 0;
		for(std::size_t j = i; j < number.size() + i || carry > 0; j++){
			
			char thisnumberdigit = j-i < number.size() ? number[j-i] : 0;
			
			char intermediaryresult = (digit1 * thisnumberdigit + carry + result[j]);
			result[j] = intermediaryresult % 10;
			carry = intermediaryresult / 10;
		}
	}

	(*this) = result;
	while(number[number.size()-1] == 0 && number.size() > 1) number.resize(number.size()-1);

	// special case: if result is zero, the number should always be positive
	if(getNumber(false) == "0") setNegative(false);
}

void BigInteger::subtract(const BigInteger &subtrahend){
	
	bool resultNegative = compare(subtrahend) > 0 ? 0 : 1;
	
	if(isNegative() == subtrahend.isNegative()){
		subtract_digits(subtrahend);
	}else{
		add_digits(subtrahend);
	}
	
	setNegative(resultNegative);
	
	// special case: if result is zero, the number should always be positive
	if(getNumber(false) == "0") setNegative(false);
}

void BigInteger::divide(const BigInteger &divisor){
	try{
		(*this) = divide_digits(divisor, false);
	}catch(DivideByZeroException &e){
		std::cerr << e.what() << std::endl;
		throw; // https://en.cppreference.com/w/cpp/language/throw -> Notes
	}

	// special case: if result is zero, the number should always be positive
	if(getNumber(false) == "0") setNegative(false);
}

void BigInteger::mod(const BigInteger &divisor){
	// mathematically the remainder is the same as modulo ONLY FOR POSITIVE NUMBERS
	// so calling this mod is not entirely correct; this is actually the remainder; maybe ill fix this some time
	// (trivia: % is the remainder in C/C++)
	try{
		(*this) = divide_digits(divisor, true);
	}catch(DivideByZeroException &e){
		std::cerr << e.what() << std::endl;
		exit(0);
	}
}

void BigInteger::pow(BigInteger exponent){
	// SUPPORT FOR NEGATIVE NUMBER AND POWER OF ZERO
	if(exponent.getNumber(false) == "0"){BigInteger one("1"); (*this) = one; return;}
	BigInteger n((*this));
	while(exponent > "1"){
		multiply(n);
		exponent.subtract("1");
	}
}

void BigInteger::bitAnd(const BigInteger &operation_partner){
	std::string numb = getBinaryString();
	std::string opb = operation_partner.getBinaryString();
	std::string res("");
	res.resize(std::min(numb.size(), opb.size()), 0);
	
	for(int i = res.size()-1; i >= 0; i--){
		res[i] = numb[i] & opb[i];
	}

	fromBinaryString(res);
	// special case: if result is zero, the number should always be positive
	if(getNumber(false) == "0") setNegative(false);
}

void BigInteger::bitOr(const BigInteger &operation_partner){
	
	std::string numb = getBinaryString();
	std::string opb = operation_partner.getBinaryString();
	std::string res("");
	res.resize(std::max(numb.size(), opb.size()), 0);
	
	for(long int i = res.size()-1; i >= 0; i--){
		res[i] = (((long unsigned int)i < numb.size()) ? numb[i] : 0) | (((long unsigned int)i < opb.size()) ? opb[i] : 0);
	}

	fromBinaryString(res);
}

void BigInteger::bitShiftRight(int count){
	
	std::string numb = getBinaryString();
	std::string res("");
	int a = numb.size()-count;
	res.resize(a > 0 ? a : 0);
	
	res = numb.substr(count, std::string::npos);
	
	fromBinaryString(res);
}

void BigInteger::bitShiftLeft(int count){
	
	std::string numb = getBinaryString();
	/**std::string res("");
	int a = numb.size()-count;
	res.resize(a > 0 ? a : 0);
	
	res = numb.substr(0, numb.size()-1-count);
	res.insert(0, count, 0);**/

	numb.insert(0, count, 0);

	fromBinaryString(numb);
}

void BigInteger::modpow(BigInteger exp, const BigInteger &mod){
	BigInteger base(*this);
	base %= mod;
	//base = base % mod;
	BigInteger result("1");
	BigInteger one("1");
	BigInteger zero("0");

	while (exp > zero) {
		BigInteger h(exp);
		h.bitAnd(one);
		if (h != zero) result = (result * base) % mod;
		base = (base * base) % mod;
		exp.bitShiftRight(1);
	}
	
	(*this) = result;

   /* BigInteger res = 1;     // Initialize result
 
    x = x % p; // Update x if it is more than or
                // equal to p
  
    if (x == 0) return BigInteger(0); // In case x is divisible by p;
 
    while (y > 0)
    {
        // If y is odd, multiply x with result
		BigInteger yandone = y;
		yandone.bitAnd(1);
        if (not (yandone == 0))
            res = (res*x) % p;
 
        // y must be even now
        y.bitShiftLeft(1); // y = y/2
        x = (x*x) % p;
    }
    return res;*/

}



bool BigInteger::operator <(const BigInteger &cmp) const {return compare(cmp) == -1;}
bool BigInteger::operator >(const BigInteger &cmp) const {return compare(cmp) == 1;}
bool BigInteger::operator ==(const BigInteger &cmp) const {return compare(cmp) == 0;}
bool BigInteger::operator !=(const BigInteger &cmp) const {return !((*this) == cmp);}
bool BigInteger::operator <=(const BigInteger &cmp) const {char res = compare(cmp); return res == -1 || res == 0;}
bool BigInteger::operator >=(const BigInteger &cmp) const {char res = compare(cmp); return res == 1 || res == 0;}
BigInteger BigInteger::operator +(const BigInteger &b) const {
	// This uses the copy constructor https://icarus.cs.weber.edu/~dab/cs1410/textbook/14.CPP_Relationships/copying.html
	// The copy constructor deep copies the number std::string, so you do not need to worry that the underlying char arrays of the string
	// are only address copied. See here: https://stackoverflow.com/a/5563997/9298528
	BigInteger result(*this);
	result.add(b);
	return result;
}
BigInteger BigInteger::operator -(const BigInteger &b) const {
	BigInteger result(*this);
	result.subtract(b);
	return result;
}
BigInteger BigInteger::operator *(const BigInteger &b) const {
	BigInteger result(*this);
	result.multiply(b);
	return result;
}
BigInteger BigInteger::operator /(const BigInteger &b) const {
	BigInteger result(*this);
	result.divide(b);
	return result;
}
BigInteger BigInteger::operator %(const BigInteger &b) const {
	BigInteger result(*this);
	result.mod(b);
	return result;
}
BigInteger BigInteger::operator ^(const BigInteger &b) const {
	BigInteger result(*this);
	result.pow(b);
	return result;
}
BigInteger BigInteger::gcd(BigInteger a, BigInteger b) {
	BigInteger zero("0");
	for (;;) {
		if (b == zero)
			return a;
		a %= b;
		if (a == zero)
			return b;
		b %= a;
	}
}

BigInteger BigInteger::modinv(BigInteger a, BigInteger b){
	/*for(int x = 1; BigInteger(x) < m; x++)
        if(((a%m) * (BigInteger(x)%m)) % m == 1)
            return x;
	return BigInteger("0");*/
	BigInteger b0 = b, t, q;
	BigInteger x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b;
		b = a % b;
		a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}

void BigInteger::operator +=(const BigInteger &b) {add(b);}
void BigInteger::operator -=(const BigInteger &b) {subtract(b);}
void BigInteger::operator *=(const BigInteger &b) {multiply(b);}
void BigInteger::operator /=(const BigInteger &b) {divide(b);}
void BigInteger::operator %=(const BigInteger &b) {mod(b);}
void BigInteger::operator ^=(const BigInteger &b) {pow(b);}
		
char BigInteger::compare(const BigInteger &cmp) const {
	// returns -1 if this is smaller; 0 if numbers are equal; 1 if this is greater
	if(getSize() > 0 || number[0] > 0 || cmp.getSize() > 0 || cmp.at(0) > 0) {
		if (isNegative() && !cmp.isNegative()) {
			return -1;
		} else if (!isNegative() && cmp.isNegative()) {
			return 1;
		}
	}

	if(isNegative()){
		// return symmetrical counterpart if both are negative (because digits larger are smaller numbers; negative numbers grow left)
		return compare_digits(cmp)*-1;
	}else{
		return compare_digits(cmp);
	}
}

std::size_t BigInteger::getSize() const {return number.size();}
char& BigInteger::operator [](int ind){return number[ind];}
char BigInteger::at(int ind) const {return number[ind];}
void BigInteger::resize(int n){number.resize(n);}
bool BigInteger::isNegative() const {return isnegative;}
void BigInteger::setNegative(bool s){isnegative = s;}

std::string BigInteger::getNumber() const {
	return getNumber(true);
}
std::string BigInteger::getNumber(bool sign) const {
	std::string n;
	if(sign && isNegative()) n = "-";
	for(int i = number.size()-1; i >= 0; i--){
		n += number[i] + '0';
	}
	return n;
}

void BigInteger::add_digits(const BigInteger &summand2){
	std::size_t maxlengthofresult = std::max(getSize(), summand2.getSize()) + 1;
	if(maxlengthofresult > number.size()) number.resize(maxlengthofresult, 0);
	
	char carry = 0;
	
	for(std::size_t i = 0; i < summand2.getSize() || carry > 0; i++){
		
		char summand2digit = i < summand2.getSize() ? summand2.at(i) : 0;
		char summand1digit = number[i];
		
		char total = summand1digit + summand2digit + carry;
		number[i] = total % 10;
		carry = (total >= 10) ? 1 : 0;
	}
	
	// remove precociously allocated and filled leading 0 (else number grows everytime anything is added)
	while(number[number.size()-1] == 0 && number.size() > 1) number.resize(number.size()-1);
}
		
void BigInteger::subtract_digits(BigInteger subtrahend){
	BigInteger placeholder("0");
	
	if(compare_digits(subtrahend) == -1){
		placeholder = (*this);
		(*this) = subtrahend;
		subtrahend = placeholder;
	}
	
	int maxlengthofresult = std::max(getSize(), subtrahend.getSize());
	resize(maxlengthofresult);
	
	char carry = 0;
	
	for(std::size_t i = 0; i < getSize(); i++){
		char subtrahenddigit = i < subtrahend.getSize() ? subtrahend[i] : 0;
		char minuenddigit = number[i];

		number[i] = minuenddigit - subtrahenddigit + carry;
		
		if(number[i] < 0){
			carry = -1;
			number[i] += 10;
		}else{
			carry = 0;
		}
	}

	while(number[number.size()-1] == 0 && number.size() > 1) number.resize(number.size()-1);
}

char BigInteger::compare_digits(const BigInteger &cmp) const {
	// this disrespects sign and only looks at the numbers
	// returns -1 if this is smaller; 0 if numbers are equal; 1 if this is greater
	
	// first check if one number is short than the other
	if(cmp.getSize() < getSize()){return 1;}
	if(cmp.getSize() > getSize()){return -1;}

	for(int i = getSize()-1; i >= 0; i--){
		if(cmp.at(i) < number.at(i)){return 1;}
		if(cmp.at(i) > number.at(i)){return -1;}
	}
	
	// numbers are equal
	return 0;
}

BigInteger BigInteger::divide_digits(const BigInteger &divisor, bool mod){
	if(divisor == "0") throw DivideByZeroException();
	BigInteger result("0");
	if(getNumber(false) == "0") return mod ? *this : result;
	
	// determine sign
	if((isNegative() && !divisor.isNegative()) || (!isNegative() && divisor.isNegative())){result.setNegative(true);}
	
	int maxlengthofresult = std::max(getSize(), divisor.getSize());
	result.resize(maxlengthofresult);

	std::size_t numerator_index = 1;
	BigInteger partial_numerator(getNumber(false).substr(0, 1).c_str());
	for(; partial_numerator.compare_digits(divisor) == -1 && numerator_index <= getSize(); numerator_index++){
		std::string num = partial_numerator.getNumber(false) + getNumber(false).substr(numerator_index, 1);
		// https://stackoverflow.com/a/6868389/9298528
		// placement new
		// recreates partial_numerator in its own memory region but it could leak memory of pointers 
		// created on partial_numerator before the placement new so its good practice to run the 
		// destructor first
		// (&partial_numerator)->~partial_numerator(); which I wont do bc there arent any pointers (i checked)
		// and the operation makes the algorithm slower
		new (&partial_numerator) BigInteger(num.c_str());
	}
	
	int j = 0;
	for(; numerator_index < getNumber(false).size() || j == 0; j++){
	
		if(partial_numerator.compare_digits(divisor) == -1 && numerator_index <= getSize()){
			std::string num = partial_numerator.getNumber(false) + getNumber(false).substr(numerator_index, 1);

			new (&partial_numerator) BigInteger(num.c_str());
			numerator_index++;
		}
		
		while(partial_numerator[partial_numerator.getSize()-1] == 0 && partial_numerator.getSize() > 1) partial_numerator.resize(partial_numerator.getSize()-1);

		for(; partial_numerator.compare_digits(divisor) == 0 || partial_numerator.compare_digits(divisor) == 1; result[j]++){
			partial_numerator.subtract_digits(divisor);
		}
	}
	if(mod){return partial_numerator;}
	else{
		while(result[result.getSize()-1] == 0 && result.getSize() > std::max<std::size_t>(j,1)) result.resize(result.getSize()-1);
		result.invertNumber();
		return result;
	}
}

std::string BigInteger::getBinaryString() const {
	BigInteger b(*this);
	BigInteger zero("0");
	std::string result;
	BigInteger two("2");

	while(b != zero){
		result.append((b % two).number);
		b /= two;
	}
	
	return result;
}

void BigInteger::fromBinaryString(const std::string &bin){
	// preserve sign
	bool presneg = isNegative();
	multiply(0);
	for(int i = bin.size()-1; i >= 0; i--){
		BigInteger exp(i);
		BigInteger multiplicant(BigInteger(2) ^ exp);
		BigInteger summand(bin[i]);
		summand.multiply(multiplicant);	
		add(summand);
	}
	setNegative(presneg);
}

void BigInteger::invertNumber(){
	std::string n;
	for(int i = number.size()-1; i >= 0; i--){n += number[i];}
	number = n;
}

std::ostream& operator<< (std::ostream& stream, const BigInteger& b){
	stream << b.getNumber();
	return stream;
}
/*
int main(){
	
	
	BigInteger a("658754366326");
	a.printBinary();
	
	BigInteger b("9705457636");
	b.printBinary();
	
	a.bitAnd(b);
	a.printBinary();
	a.print();
	
	a.bitShiftRight(5);
	a.printBinary();
	a.print();
	
	a.bitShiftLeft(5);
	a.printBinary();
	a.print();
	
	a = "5433561313";
	a.print();
	a.modpow("53561313", "43561313");
	a.print();
	
	return 0;
}
*/
