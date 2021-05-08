#include "UnitConversion.h"
#include <iostream>


UnitConversion::UnitConversion()
	:m_CountOfNames(0)
{
	m_GraphEdges.emplace_back();
}

UnitConversion::UnitConversion(const UnitConversion& obj)
	: m_CountOfNames(obj.m_CountOfNames)
{
	Allocate(obj);
}

UnitConversion& UnitConversion::operator=(const UnitConversion& obj)
{
	if (this != &obj)
	{
		Deallocate();
		Allocate(obj);
	}
	return *this;
}

UnitConversion::~UnitConversion()
{
	Deallocate();
}

void UnitConversion::Conversion(std::string& s)
{
	if (s == "")
		return;
	int len = s.size();
	std::string str[4];
	int ind = 0;
	for (int i = 0; i < len; ++i)
	{
		if (s[i] == ' ')
			ind++;
		else if (s[i] == '=')
			ind--;
		else
		{
			str[ind] += s[i];
		}
	}
	if (str[2] == "?")
		GetConversion(std::stod(str[0]), str[1], str[3]);
	else
		PushConversion(std::stod(str[0]), str[1], std::stod(str[2]), str[3]);
}

void UnitConversion::Allocate(const UnitConversion& obj)
{
	m_CountOfNames = obj.m_CountOfNames;
	for (auto& ind : obj.m_Index)
	{
		m_Index[ind.first] = ind.second;
	}
	for (auto& edgevector : obj.m_GraphEdges)
	{
		m_GraphEdges.push_back(edgevector);
	}
}

void UnitConversion::Deallocate()
{
	m_Index.clear();
	m_GraphEdges.clear();
}

void UnitConversion::PushConversion(double value1, std::string& name1, double value2, std::string& name2)
{
	if (m_Index[name1] == 0)
		m_Index[name1] = ++m_CountOfNames;
	if (m_Index[name2] == 0)
		m_Index[name2] = ++m_CountOfNames;
	int ind1 = m_Index[name1], ind2 = m_Index[name2];
	if (int(m_GraphEdges.size()) <= ind1 || int(m_GraphEdges.size()) <= ind2 || Find(ind1, ind2, 0, 1.0) == 0)
	{
		if (int(m_GraphEdges.size()) <= ind1)
			m_GraphEdges.emplace_back();
		if (int(m_GraphEdges.size()) <= ind2)
			m_GraphEdges.emplace_back();
		m_GraphEdges[ind1].emplace_back(ind2, value2 / value1);
		m_GraphEdges[ind2].emplace_back(ind1, value1 / value2);
	}
}

void UnitConversion::GetConversion(double value, std::string& name1, std::string& name2)
{
	int ind1 = m_Index[name1], ind2 = m_Index[name2];
	double value2 = Find(ind1, ind2, 0, 1.0) * value;
	if (value2 != 0)
		std::cout << value << " " << name1 << " = " << value2 << " " << name2 << std::endl;
	else
		std::cout << "No conversion is possible." << std::endl;
}

double UnitConversion::Find(int node, int searchnode, int parent, double value)
{
	for (auto& to : m_GraphEdges[node])
	{
		if (to.first == parent)
			continue;
		if (to.first == searchnode)
			return value * to.second;
		double val = Find(to.first, searchnode, node, value * to.second);
		if (val != 0)
			return val;
	}
	return 0;
}