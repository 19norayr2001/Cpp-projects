#include "UnitTests.h"
#include "../Spreadsheet/Spreadsheet.h"
#include <iostream>
#include <fstream>
#include <vector>

void Testing::SpreadsheetSimulatorTesting()
{
	const int size = 3;
	Spreadsheet Simulator;
	std::cout.setf(std::ios::left);
	for (int i = 0; i < size; ++i)
	{
		char ch = i + 1 + '0';
		std::string filename = "./Tests/test";
		filename += ch;
		filename += "in.txt";
		std::ifstream fin(filename);
		int n{}, m{};
		std::cout << "Test" << " " << i + 1 << std::endl << std::endl;
		fin >> n >> m;
		std::cout << n << " " << m << std::endl;
		std::vector<std::vector<std::string>> matrix(n, std::vector<std::string>(m));
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				fin >> matrix[i][j];
				std::cout.width(10);
				std::cout << matrix[i][j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		Simulator.NewSpreadsheet(n, m, matrix);
		std::cout << Simulator << std::endl;
		fin.close();
	}
}