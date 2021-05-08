#include <iostream>
#include <vector>
#include "UnitTests.h"
#include "..\Pointers\UniquePointer.h"
#include "..\Pointers\SharedPointer.h"

void Testing::TestingUniquePointer()
{
	UniquePointer<int> myptr(new int(7));
	std::cout << *myptr << std::endl;
	*myptr += 7;
	const UniquePointer<int> ptr(myptr);
	std::cout << *ptr << std::endl;
	UniquePointer<std::vector<int>> myvec(new std::vector<int>);
	myvec->push_back(7);
	myvec->push_back(10);
	myvec->push_back(-1);
	for (auto x : *myvec)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
}
void Testing::TestingSharedPointer()
{
	SharedPointer<int> ptr(new int(4));
	SharedPointer<int> ptr2(ptr);
	std::cout << *ptr << std::endl;
	std::cout << *ptr2 << std::endl;
	ptr2 = SharedPointer<int>(new int(10));
	*ptr2 -= 5;
	std::cout << *ptr2 << std::endl;
}