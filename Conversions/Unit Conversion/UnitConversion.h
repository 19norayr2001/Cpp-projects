#pragma once
#include <string>
#include <vector>
#include <unordered_map>
class UnitConversion
{
public:
	UnitConversion();
	UnitConversion(const UnitConversion&);
	UnitConversion& operator=(const UnitConversion&);
	~UnitConversion();
	void Conversion(std::string&);
private:
	void Allocate(const UnitConversion&);
	void Deallocate();
	void PushConversion(double, std::string&, double, std::string&);
	void GetConversion(double, std::string&, std::string&);
	double Find(int, int, int, double);
private:
	int m_CountOfNames;
	std::unordered_map<std::string, int> m_Index;
	std::vector<std::vector<std::pair<int, double>>> m_GraphEdges;
};

