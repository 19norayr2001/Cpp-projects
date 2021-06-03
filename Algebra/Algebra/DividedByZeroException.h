#pragma once
#include <exception>
class DividedByZeroException : public std::exception {
public:
	DividedByZeroException()
		:std::exception("Not allowed divide by zero") {}
};

