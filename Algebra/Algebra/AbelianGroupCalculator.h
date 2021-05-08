#pragma once
#include <vector>

class AbelianGroupCalculator
{
	typedef std::vector<std::pair<int, int>> PrimePowerVector;
private:
	static AbelianGroupCalculator instance;
	AbelianGroupCalculator() {}
public:
	static const AbelianGroupCalculator& getInstance();
public:
	long long calculateCount(int) const;
private:
	std::vector<std::pair<int, int>> getPrimePowers(int) const;
	long long calculateForPrimePower(int) const;
public:
	bool isIsomorph(const std::vector<int>&, const std::vector<int>&) const;
};

