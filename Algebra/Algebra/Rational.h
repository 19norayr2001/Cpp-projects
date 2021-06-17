#pragma once
#include <iostream>

class Rational {
	friend std::ostream& operator<<(std::ostream&, const Rational&);
	friend std::istream& operator>>(std::istream&, Rational&);
public:
	Rational(long long = 0, long long = 1);
public:
	Rational& operator*=(const Rational&);
	Rational& operator*=(long long);
	Rational& operator/=(const Rational&);
	Rational& operator+=(const Rational&);
	Rational& operator-=(const Rational&);
	bool operator==(const Rational&) const;
private:
	static long long gcd(long long, long long);
	//correct numerator and denominator, as they can not have common divisor;
	void correct();
private:
	long long numerator;
	long long denominator;
};


Rational operator*(Rational, const Rational&);
Rational operator*(long long, Rational);
Rational operator*(Rational, long long);
Rational operator/(Rational, const Rational&);
Rational operator+(Rational, const Rational&);
Rational operator-(Rational, const Rational&);
Rational operator-(const Rational&);
bool operator!=(const Rational&, const Rational&);