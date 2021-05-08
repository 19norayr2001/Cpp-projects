#include "UnitTests.h"
#include "Treap.h"
#include "ImplicitTreap.h"
#include <iostream>
#include <chrono>
#include <random>


void Testing::TreapTesting()
{
	std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
	Treap set;
	int n = 0, m = 0, k = 0;
	std::cout << "Input array size" << std::endl;
	std::cin >> n;
	std::cout << "Input array" << std::endl;
	ImplicitTreap vec;
	for (int i = 0; i < n; ++i)
	{
		std::cin >> k;
		vec.pushBack(k);
	}
	std::cout << "Input reverse query count" << std::endl;
	std::cin >> m;
	while (m--)
	{
		int a, b;
		std::cin >> a >> b;
#ifdef TREAP_REVERSE 
		vec.reverse(a - 1, b - 1);
#endif // TREAP_REVERSE
		std::cout << "Array from query" << std::endl;
		for (size_t i = 0; i < vec.size(); ++i)
		{
			std::cout << vec.keyOfOrder(i) << " ";
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
	std::cout << "Input insert query count" << std::endl;
	std::cin >> m;
	while (m--)
	{
		int a, b;
		std::cin >> a >> b;
		vec.insert(b, a - 1);
		std::cout << "Array from query" << std::endl;
		for (size_t i = 0; i < vec.size(); ++i)
		{
			std::cout << vec.keyOfOrder(i) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Input delete query count" << std::endl;
	std::cin >> m;
	while (m--)
	{
		int a;
		std::cin >> a;
		vec.remove(a - 1);
		std::cout << "Array from query" << std::endl;
		for (size_t i = 0; i < vec.size(); ++i)
		{
			std::cout << vec.keyOfOrder(i) << " ";
		}
		std::cout << std::endl;
	}
}