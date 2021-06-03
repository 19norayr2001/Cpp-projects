#include "Rational.h"
#include <stdexcept>

Rational operator*(Rational obj, const Rational& obj2) {
	return obj *= obj2;
}

Rational operator*(Rational obj, int n) {
	return obj *= n;
}

Rational operator*(int n, Rational obj) {
	return obj *= n;
}

Rational operator/(Rational obj, const Rational& obj2) {
	return obj /= obj2;
}

Rational operator+(Rational obj, const Rational& obj2) {
	return obj += obj2;
}

Rational operator-(Rational obj, const Rational& obj2) {
	return obj -= obj2;
}

Rational operator-(const Rational& r) {
	Rational r1;
	return r1 -= r;
}

bool operator!=(const Rational& r, const Rational& r1) {
	return !(r == r1);
}


std::ostream& operator<<(std::ostream& out, const Rational& r) {
	out << r.numerator;
	if (r.denominator != 1) {
		out << '/' << r.denominator;
	}
	return out;
}

Rational::Rational(int n, int m)
	:numerator(n)
	, denominator(m) {

	if (m == 0) {
		throw std::invalid_argument("Invalid argument, rational number denominator cannot be zero");
	}
	correct();
}

Rational& Rational::Rational::operator*=(const Rational& obj) {
	numerator *= obj.numerator;
	denominator *= obj.denominator;
	correct();
	return *this;
}

Rational& Rational::operator*=(int n) {
	return *this *= Rational(n, 1);
}

Rational& Rational::operator/=(const Rational& obj) {
	// can't divide by zero, so throw an exception
	if (obj.numerator == 0) {
		throw std::invalid_argument("Invalid argument, rational number denominator cannot be zero");
	}
	if (this == &obj) {
		return *this = 1;
	}
	numerator *= obj.denominator;
	denominator *= obj.numerator;
	correct();
	return *this;
}

Rational& Rational::operator+=(const Rational& obj) {
	// p1 / q1 + p2 / q2 = (p1 * q2 + p2 * q1) / (q1 * q2)
	numerator = numerator * obj.denominator + denominator * obj.numerator;
	denominator *= obj.denominator;
	correct();
	return *this;
}

Rational& Rational::operator-=(const Rational& obj) {
	// p1 / q1 + p2 / q2 = (p1 * q2 + p2 * q1) / (q1 * q2)
	numerator = numerator * obj.denominator - denominator * obj.numerator;
	denominator *= obj.denominator;
	correct();
	return *this;
}


bool Rational::operator==(const Rational& r) const {
	return numerator * r.denominator == denominator * r.numerator;
}

// get gcd of two numbers with Euclidean algorithm
int Rational::gcd(int a, int b) {
	if (b == 0) {
		return a;
	}
	return gcd(b, a % b);
}

void Rational::correct() {
	// get their gcd and divide on it numerator and denominator
	int _gcd = gcd(numerator, denominator);
	numerator /= _gcd;
	denominator /= _gcd;

	if (denominator < 0) {
		numerator *= -1;
		denominator *= -1;
	}
}