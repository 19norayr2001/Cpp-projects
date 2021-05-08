#include "Permutation.h"

Permutation::Permutation(const std::vector<int>& perm) 
	:mPermuation(perm){}

Permutation& Permutation::operator*=(const Permutation& permutation) {
	for (size_t i = 0; i < mPermuation.size(); ++i) {
		mPermuation[i] = permutation[mPermuation[i]];
	}
	return *this;
}

bool Permutation::operator==(const Permutation& p) {
	return p.mPermuation == mPermuation;
}

int Permutation::operator[](size_t i) const {
	return mPermuation[i - 1];
}

Permutation operator*(Permutation perm1, const Permutation& perm2) {
	return perm1 *= perm2;
}

Permutation operator-(const Permutation& permutation) {
	std::vector<int> vec(permutation.size());
	for (size_t i = 0; i < permutation.size(); ++i) {
		vec[permutation[i + 1] - 1] = i + 1;
	}
	return Permutation(vec);
}

std::ostream& operator<<(std::ostream& out, const Permutation& permutation) {
	out << '(' << permutation[1];
	for (size_t i = 2; i <= permutation.size(); ++i) {
		out << ", " << permutation[i];
	}
	out << ')';
	return out;
}