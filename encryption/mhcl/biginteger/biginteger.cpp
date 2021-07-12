#include "biginteger.h"

class DivideByZeroException : public std::exception{
	public:
		const char* what() const noexcept{
			return "Division by zero is unsupported behaviour.";
		}
		~DivideByZeroException(){}
};


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
	for(int i = size-1; i >= lowerbound; i--){
		number[size-1-i] = n[i] - '0';
	}
}

BigInteger::BigInteger(std::string s) : BigInteger(s.c_str()){}
BigInteger::BigInteger(int i) : BigInteger(std::to_string(i)){}

void BigInteger::print() const {
	if(isnegative)std::cout << '-';
	
	for(int i = number.size()-1; i >= 0; i--){
		std::cout << (int)(number[i]);
	}
	std::cout << std::endl;
}

void BigInteger::printBinary() const { // INVERT PRINTOUT
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
}

// O(n*m)
void BigInteger::multiply(const BigInteger &multiplicator2){
	BigInteger result("0");
	// if one of the multiplicators is negative but not both, the result is negative
	if((multiplicator2.isNegative() || isnegative) && !(multiplicator2.isNegative() && isnegative)) result.setNegative(true);
	
	int maxlengthofresult = (int)number.size() + multiplicator2.getSize();
	result.resize(maxlengthofresult);
	
	for(int i = 0; i < multiplicator2.getSize(); i++){
		char digit1 = multiplicator2.at(i);
		char carry = 0;
		for(int j = i; j < number.size() + i || carry > 0; j++){
			
			char thisnumberdigit = j-i < number.size() ? number[j-i] : 0;	
			
			char intermediaryresult = (digit1 * thisnumberdigit + carry + result[j]);
			result[j] = intermediaryresult % 10;
			carry = intermediaryresult / 10;
		}
	}

	(*this) = result;
	while(number[number.size()-1] == 0 && number.size() > 1) number.resize(number.size()-1);
}

void BigInteger::subtract(const BigInteger &subtrahend){
	
	bool resultNegative = compare(subtrahend) > 0 ? 0 : 1;
	
	if(isNegative() == subtrahend.isNegative()){
		subtract_digits(subtrahend);
	}else{
		add_digits(subtrahend);
	}
	
	setNegative(resultNegative);
}

void BigInteger::divide(const BigInteger &divisor){
	try{
		(*this) = divide_digits(divisor, false);
	}catch(DivideByZeroException &e){
		std::cerr << e.what() << std::endl;
		exit(0);
	}
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
	if(exponent.getNumber() == "0"){BigInteger one("1"); (*this) = one; return;}
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
	
	for(int i = numb.size()-1; i >= 0; i--){
		res[i] = numb[i] & opb[i];
	}
	//std::cout << res << std::endl;
	
	multiply(0);
	for(int i = res.size()-1; i >= 0; i--){
		BigInteger exp(i);
		BigInteger multiplicant(BigInteger(2) ^ exp);
		BigInteger summand(res[i]);
		summand.multiply(multiplicant);
		
		add(summand);
	}
	int a = 5;
	
}

void BigInteger::modpow(const BigInteger &divisor, const BigInteger &exponent){}

bool BigInteger::operator <(const BigInteger &cmp) const {return compare(cmp) == -1;}
bool BigInteger::operator >(const BigInteger &cmp) const {return compare(cmp) == 1;}
bool BigInteger::operator ==(const BigInteger &cmp) const {return compare(cmp) == 0;}
bool BigInteger::operator !=(const BigInteger &cmp) const {return !((*this) == cmp);}
bool BigInteger::operator <=(const BigInteger &cmp) const {char res = compare(cmp); return res == -1 || res == 0;}
bool BigInteger::operator >=(const BigInteger &cmp) const {char res = compare(cmp); return res == 1 || res == 0;}
BigInteger BigInteger::operator +(const BigInteger &cmp) const {
	// This uses the copy constructor https://icarus.cs.weber.edu/~dab/cs1410/textbook/14.CPP_Relationships/copying.html
	// The copy constructor deep copies the number std::string, so you do not need to worry that the underlying char arrays of the string
	// are only address copied. See here: https://stackoverflow.com/a/5563997/9298528
	BigInteger result(*this);
	result.add(cmp);
	return result;
}
BigInteger BigInteger::operator -(const BigInteger &cmp) const {
	BigInteger result(*this);
	result.subtract(cmp);
	return result;
}
BigInteger BigInteger::operator *(const BigInteger &cmp) const {
	BigInteger result(*this);
	result.multiply(cmp);
	return result;
}
BigInteger BigInteger::operator /(const BigInteger &cmp) const {
	BigInteger result(*this);
	result.divide(cmp);
	return result;
}
BigInteger BigInteger::operator %(const BigInteger &cmp) const {
	BigInteger result(*this);
	result.mod(cmp);
	return result;
}
BigInteger BigInteger::operator ^(const BigInteger &cmp) const {
	BigInteger result(*this);
	result.pow(cmp);
	return result;
}
void BigInteger::operator +=(const BigInteger &cmp) {add(cmp);}
void BigInteger::operator -=(const BigInteger &cmp) {subtract(cmp);}
void BigInteger::operator *=(const BigInteger &cmp) {multiply(cmp);}
void BigInteger::operator /=(const BigInteger &cmp) {divide(cmp);}
void BigInteger::operator %=(const BigInteger &cmp) {mod(cmp);}
void BigInteger::operator ^=(const BigInteger &cmp) {pow(cmp);}
		
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

int BigInteger::getSize() const {return number.size();}
char& BigInteger::operator [](int ind){return number[ind];}
char BigInteger::at(int ind) const {return number[ind];}
void BigInteger::resize(int n){number.resize(n);}
bool BigInteger::isNegative() const {return isnegative;}
void BigInteger::setNegative(bool s){isnegative = s;}

std::string BigInteger::getBinaryString() const {
	BigInteger b(*this);
	BigInteger zero("0");
	std::string result;
	
	if(getNumber() == "13642"){
		int a = 5;
	}
	
	while(b != zero){
		BigInteger t(b);
		BigInteger two(2);
		t.mod(two);
		std::string s = t.number;
		result.append(s);
		b /= 2;
	}
	
	return result;
}
std::string BigInteger::getNumber() const {
	std::string n;
	for(int i = number.size()-1; i >= 0; i--){
		n += number[i] + '0';
	}
	return n;
}

void BigInteger::add_digits(const BigInteger &summand2){
	int maxlengthofresult = std::max(getSize(), summand2.getSize()) + 1;
	if(maxlengthofresult > number.size()) number.resize(maxlengthofresult, 0);
	
	char carry = 0;
	
	for(int i = 0; i < summand2.getSize() || carry > 0; i++){
		
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
	
	for(int i = 0; i < getSize(); i++){
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
	if(getNumber() == "0") throw DivideByZeroException();
	BigInteger result("0");
	if(divisor == "0") return mod ? *this : result;
	
	// determine sign
	if((isNegative() && !divisor.isNegative()) || (!isNegative() && divisor.isNegative())){result.setNegative(true);}
	
	int maxlengthofresult = std::max(getSize(), divisor.getSize());
	result.resize(maxlengthofresult);

	int numerator_index = 1;
	BigInteger partial_numerator(getNumber().substr(0, 1).c_str());
	for(; partial_numerator.compare_digits(divisor) == -1 && numerator_index <= getSize(); numerator_index++){
		std::string num = partial_numerator.getNumber() + getNumber().substr(numerator_index, 1);
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
	for(; numerator_index < getNumber().size() || j == 0; j++){
	
		if(partial_numerator.compare_digits(divisor) == -1 && numerator_index <= getSize()){
			std::string num = partial_numerator.getNumber() + getNumber().substr(numerator_index, 1);

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
		while(result[result.getSize()-1] == 0 && result.getSize() > std::max(j,1)) result.resize(result.getSize()-1);
		result.invertNumber();
		return result;
	}
}

void BigInteger::invertNumber(){
	std::string n;
	for(int i = number.size()-1; i >= 0; i--){n += number[i];}
	number = n;
}


int main(){
	
	
	BigInteger a("658754366326");
	a.printBinary();
	
	BigInteger b("9705457636");
	b.printBinary();
	
	a.bitAnd(b);
	a.printBinary();
	a.print();
	
	return 0;
}