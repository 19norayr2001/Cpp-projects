#pragma once
#include <exception>
class DividedByZeroException : public std::exception {
public:
	DividedByZeroException() {}

	const char* what() const noexcept override {
		return message;
	}
private:
	const char* message = "Not allowed divide by zero";
};

