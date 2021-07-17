#ifndef DIVIDEBYZEROEXCEPTION_H_INCLUDED
#define DIVIDEBYZEROEXCEPTION_H_INCLUDED


#include <exception>

class DivideByZeroException : public std::exception{
	public:
		const char* what() const noexcept;
		~DivideByZeroException();
};

#endif // DIVIDEBYZEROEXCEPTION_H_INCLUDED