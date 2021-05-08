#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "UnitTests.h"
#include "../File Manager/FileManager.h"
void Testing::FileMenegerEmulatorTesting()
{
	for (int i = 7; i <= 9; ++i)
	{
		std::stringstream ss;
		ss << "../Tests/test" << i << ".txt";
		std::ifstream fin(ss.str());
		FileManager fm;
		while (fin)
		{
			try
			{
				fin >> fm;
				std::cout << fm << std::endl;
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		std::cout << std::string(30, '-') << std::endl;
	}
}