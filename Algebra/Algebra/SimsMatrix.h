#pragma once
#include "Permutation.h"
#include <ostream>
#include <vector>
#include <tuple>

class SimsMatrix
{
	friend std::ostream& operator<<(std::ostream&, const SimsMatrix&);
	typedef std::shared_ptr<Permutation> PermutationPtr;
public:
	SimsMatrix(const std::vector<Permutation>&);
public:
	bool isGroupMember(const Permutation&) const;
	size_t getGroupSize() const;
private:
	void initMatrix(size_t);
	void cascade(const Permutation&);
	std::tuple<int, int, Permutation> getCascadeCoords(Permutation) const;
	size_t getSize() const { return mMatrix[0][0]->size(); }
private:
	std::vector<std::vector<PermutationPtr>> mMatrix;
};

