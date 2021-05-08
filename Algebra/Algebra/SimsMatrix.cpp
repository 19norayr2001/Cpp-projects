#include "SimsMatrix.h"
#include <sstream>

SimsMatrix::SimsMatrix(const std::vector<Permutation>& permutationVector) {
	initMatrix(permutationVector[0].size());
	for (const Permutation& permutation : permutationVector) {
		cascade(permutation);
	}
}

void SimsMatrix::initMatrix(size_t size) {
	mMatrix = std::vector<std::vector<PermutationPtr>>(size, std::vector<PermutationPtr>(size, nullptr));
	std::vector<int> unit(size);
	for (size_t i = 0; i < size; ++i) {
		unit[i] = i + 1;
	}
	for (size_t i = 0; i < size; ++i) {
		mMatrix[i][i] = PermutationPtr(new Permutation(unit));
	}
}

bool SimsMatrix::isGroupMember(const Permutation& permutation) const {
	auto coords = getCascadeCoords(permutation);
	int x = std::get<0>(coords);
	int y = std::get<1>(coords);
	return x == -1 && y == -1;
}

size_t SimsMatrix::getGroupSize() const {
	size_t ret = 1;
	for (size_t i = 0; i < getSize(); ++i) {
		size_t sz = 1;
		for (size_t j = i + 1; j < getSize(); ++j) {
			sz += (mMatrix[i][j] != nullptr);
		}
		ret *= sz;
	}
	return ret;
}

void SimsMatrix::cascade(const Permutation& permutation) {
	auto coords = getCascadeCoords(permutation);
	int x = std::get<0>(coords);
	int y = std::get<1>(coords);
	if (x == -1 && y == -1) {
		return;
	}
	mMatrix[x][y] = PermutationPtr(new Permutation(std::get<2>(coords)));
	for (size_t i = 0; i < getSize(); ++i) {
		for (size_t j = i + 1; j < getSize(); ++j) {
			if (mMatrix[i][j] == nullptr) {
				continue;
			}
			cascade((*mMatrix[x][y]) * (*mMatrix[i][j]));
			cascade((*mMatrix[i][j]) * (*mMatrix[x][y]));
		}
	}
}

std::tuple<int, int, Permutation> SimsMatrix::getCascadeCoords(Permutation permutation) const {
	size_t size = getSize();
	for (size_t i = 0; i < size; ++i) {
		if (mMatrix[i][permutation[i + 1] - 1] == nullptr) {
			return { i, permutation[i + 1] - 1, permutation };
		}
		permutation *= -(*mMatrix[i][permutation[i + 1] - 1]);
	}
	return { -1, -1, permutation };
}

std::ostream& operator<<(std::ostream& out, const SimsMatrix& matrix) {
	std::stringstream ss;
	ss << *matrix.mMatrix[0][0];
	size_t size = ss.str().size();
	for (size_t i = 0; i < matrix.getSize(); ++i) {
		for (size_t j = 0; j < matrix.getSize(); ++j) {
			if (matrix.mMatrix[i][j] == nullptr) {
				out << std::string(size, '#') << ' ';
				continue;
			}
			out << *matrix.mMatrix[i][j] << ' ';
		}
		out << std::endl;
	}
	return out;
}