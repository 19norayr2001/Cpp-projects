#pragma once
#include <iostream>

class Rational {
	friend std::ostream& operator<<(std::ostream&, const Rational&);
public:
	Rational(int = 0, int = 1);
public:
	Rational& operator*=(const Rational&);
	Rational& operator*=(int);
	Rational& operator/=(const Rational&);
	Rational& operator+=(const Rational&);
	Rational& operator-=(const Rational&);
	bool operator==(const Rational&) const;
private:
	static int gcd(int, int);
	//correct numerator and denominator, as they can not have common divisor;
	void correct();
private:
	int numerator;
	int denominator;
};


Rational operator*(Rational, const Rational&);
Rational operator*(int, Rational);
Rational operator*(Rational, int);
Rational operator/(Rational, const Rational&);
Rational operator+(Rational, const Rational&);
Rational operator-(Rational, const Rational&);
Rational operator-(const Rational&);
bool operator!=(const Rational&, const Rational&);