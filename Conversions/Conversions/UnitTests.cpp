#include <iostream>
#include <fstream>
#include <string>
#include "UnitTests.h"
#include "..\Unit Conversion\UnitConversion.h"
void Testing::UnitConversionTests()
{
	std::string tests[] = { "./Tests/default.in", "./Tests/test.in", "./Tests/test1.in" };
	for (auto test : tests)
	{
		std::ifstream fin(test);
		std::string s;
		UnitConversion obj;
		while (getline(fin, s))
		{
			std::cout << s << std::endl;
			obj.Conversion(s);
		}
		fin.close();
		std::cout << std::endl;
	}
}