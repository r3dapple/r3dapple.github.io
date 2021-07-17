#include "termicolor.hpp"

termicolor::termicolor(){
    std::mt19937 gen(device());
	generator = gen;
    std::uniform_int_distribution<int> dist(1,2000);
	distribution = dist;
}

std::string termicolor::randomColor(){
	int col =  distribution(generator) % 8;
	switch(col){
		case 0: return COUT_DARK_GREEN;
		case 1: return COUT_RED;
		case 2: return COUT_GREEN;
		case 3: return COUT_ORANGE;
		case 4: return COUT_BLUE;
		case 5: return COUT_MAGENTA;
		case 6: return COUT_CYAN;
		case 7: return COUT_WHITE;
		default: return "";
	}
}

std::string termicolor::darkgreen(){return COUT_DARK_GREEN;}
std::string termicolor::red(){return COUT_RED;}
std::string termicolor::green(){return COUT_GREEN;}
std::string termicolor::orange(){return COUT_ORANGE;}
std::string termicolor::blue(){return COUT_BLUE;}
std::string termicolor::magenta(){return COUT_MAGENTA;}
std::string termicolor::cyan(){return COUT_CYAN;}
std::string termicolor::white(){return COUT_WHITE;}
std::string termicolor::reset(){return COUT_RESET;}
std::string termicolor::uwhite(){return COUT_UWHITE;}
