#include "dividebyzeroexception.hpp"

const char* DivideByZeroException::what() const noexcept{
    return "Division by zero is unsupported behavior.";
}

DivideByZeroException::~DivideByZeroException(){}