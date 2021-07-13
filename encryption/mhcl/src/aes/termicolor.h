#ifndef _TERMICOLOR_H_
#define _TERMICOLOR_H_

#define COUT_DARK_GREEN "\033[1;30m"
#define COUT_RED  "\033[1;31m"
#define COUT_GREEN  "\033[1;32m"
#define COUT_ORANGE  "\033[1;33m"
#define COUT_BLUE  "\033[1;34m"
#define COUT_MAGENTA  "\033[1;35m"
#define COUT_CYAN  "\033[1;36m"
#define COUT_WHITE  "\033[1;37m"
#define COUT_RESET "\033[0m"

#define COUT_UWHITE "\033[4;37m"

#include <random>
#include <string>
#include <ctime>
#include <iostream>
class termicolor{
	public:
		termicolor();
		std::string randomColor();
		std::string darkgreen();
		std::string red();
		std::string green();
		std::string orange();
		std::string blue();
		std::string magenta();
		std::string cyan();
		std::string white();
		std::string reset();

		std::string uwhite();
	private:
		std::random_device device;
		std::mt19937 generator;
		std::uniform_int_distribution<int> distribution;
};


#endif
