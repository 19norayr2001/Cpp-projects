#include "UnitTests.h"
#include "SegmentTree.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <random>
void Testing::SegmentTreeTesting()
{
	std::vector<int> v{ 1, 2, 3, 4, 5 };

	SegmentTree<int, std::plus<int>> seg(v.begin(), v.end(), std::plus<int>());
	std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
	std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
	for (size_t i = 0; i < v.size(); ++i)
	{
		for (size_t j = i; j < v.size(); ++j)
		{
			std::cout << "Sum of segment [" << i << ", " << j << "] - " << seg.query(i, j) << std::endl;
			size_t ind = rng() % v.size();
			size_t value = rng() % 20;
			v[ind] = value;
			std::cout << std::endl;
			std::cout << "array from update ";
			copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
			seg.update(ind, value);
		}
	}
}