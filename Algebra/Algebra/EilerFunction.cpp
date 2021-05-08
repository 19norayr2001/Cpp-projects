#include "EilerFunction.h"

EilerFunction EilerFunction::instance;

namespace {
	int gcd(int a, int b) {
		return (b == 0 ? a : gcd(b, a % b));
	}
}

const EilerFunction& EilerFunction::getInstance() {
	return instance;
}

int EilerFunction::calculate(int number) const {
	int result = 0;
	for (int i = 1; i < number; ++i) {
		result += (gcd(i, number) == 1);
	}
	return result;
}