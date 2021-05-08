#include "AbelianGroupCalculator.h"
#include <algorithm>
#include <cmath>
#include <vector>

AbelianGroupCalculator AbelianGroupCalculator::instance;

const AbelianGroupCalculator& AbelianGroupCalculator::getInstance() {
	return instance;
}

long long AbelianGroupCalculator::calculateCount(int groupSize) const {
	auto powers = getPrimePowers(groupSize);
	long long answer = 1;

	for (std::pair<int, int> primePower : powers) {
		answer *= calculateForPrimePower(primePower.second);
	}
	return answer;
}

AbelianGroupCalculator::PrimePowerVector AbelianGroupCalculator::getPrimePowers(int groupSize) const {
	PrimePowerVector result;
	int n = groupSize;
	for (int i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			result.emplace_back(i, 0);
			while (n % i == 0) {
				result.back().second++;
				n /= i;
			}
		}
	}
	if (n != 1) {
		result.emplace_back(n, 1);
	}
	return result;
}

long long AbelianGroupCalculator::calculateForPrimePower(int power) const {
	std::vector<std::vector<long long>> ans(power + 1, std::vector<long long>(power + 1));
	for (int i = 1; i <= power; ++i) {
		ans[i][1] = 1;
		ans[i][i] = 1;
	}
	for (int j = 2; j <= power; ++j) {
		for (int i = 2; i < j; ++i) {
			for (int t = 0; (t + 1) * (i) <= (j); ++t) {
				ans[j][i] += ans[j - (i)*t - 1][i - 1];
			}
		}
	}
	long long answer = 0;
	for (int i = 1; i <= power; ++i)
	{
		answer += ans[power][i];
	}
	return answer;
}

bool AbelianGroupCalculator::isIsomorph(const std::vector<int>& group1, const std::vector<int>& group2) const {
	PrimePowerVector primePowers1, primePowers2;
	for (int number : group1) {
		PrimePowerVector vec = getPrimePowers(number);
		for (auto p : vec) {
			primePowers1.push_back(p);
		}
	}

	for (int number : group2) {
		PrimePowerVector vec = getPrimePowers(number);
		for (auto p : vec) {
			primePowers2.push_back(p);
		}
	}

	std::sort(primePowers1.begin(), primePowers1.end());
	std::sort(primePowers2.begin(), primePowers2.end());

	if (primePowers1.size() != primePowers2.size()) {
		return false;
	}

	for (size_t i = 0; i < primePowers1.size(); ++i) {
		if (primePowers1[i] != primePowers2[i]) {
			return false;
		}
	}

	return true;
}