#pragma once
#include <vector>
#include <ostream>
class Permutation {
public:
	Permutation(const std::vector<int>&);
public:
	Permutation& operator*=(const Permutation&);
	bool operator==(const Permutation&);
	int operator[](size_t) const;
	size_t size() const { return mPermuation.size(); }
private:
	std::vector<int> mPermuation;
};

Permutation operator*(Permutation, const Permutation&);
Permutation operator-(const Permutation&);
std::ostream& operator<<(std::ostream&, const Permutation&);

