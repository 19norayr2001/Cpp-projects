#include "Spreadsheet.h"
#include <string>

namespace
{
	bool isnumber(const std::string& s)
	{
		int len = s.size();
		short start = 0;
		if (len > 0 && s[0] == '-')
		{
			len--;
			start++;
		}
		for (int i = start; i < start + len; ++i)
		{
			if (!isdigit(s[i]))
			{
				return false;
			}
		}
		return len != 0;
	}

	bool isoption(char ch)
	{
		return ch == '+' || ch == '-' || ch == '*' || ch == '/';
	}

	int Option(int n1, char op, int n2)
	{
		if (op == '+')
			return n1 + n2;
		if (op == '-')
			return n1 - n2;
		if (op == '*')
			return n1 * n2;
		if (op == '/')
			return n1 / n2;
	}

	bool iscell(const std::string& s)
	{
		return s.size() == 2 && isdigit(s[1]) && isupper(s[0]);
	}
}

Spreadsheet::Spreadsheet()
{}

Spreadsheet::Spreadsheet(const Spreadsheet& obj)
{
	Allocate(obj);
}

Spreadsheet& Spreadsheet::operator=(const Spreadsheet& obj)
{
	if (this != &obj)
	{
		Deallocate();
		Allocate(obj);
	}
	return *this;
}

Spreadsheet::~Spreadsheet()
{
	Deallocate();
}

void Spreadsheet::NewSpreadsheet(int n, int m, const Matrix<std::string>& matrix)
{
	Deallocate();
	m_Sheet = Matrix<Cell*>(n, std::vector<Cell*>(m));
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			Parsing(i, j, matrix[i][j]);
		}
	}
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			if(!m_Sheet[i][j]->IsCalculated())
				m_Sheet[i][j]->Calculate();
		}
	}
}

void Spreadsheet::Parsing(int i, int j, const std::string& s)
{
	if (s[0] == '\'')
	{
		m_Sheet[i][j] = new Text(s, &m_Sheet);
		return;
	}
	if (isnumber(s))
	{
		m_Sheet[i][j] = new Number(s, &m_Sheet);
		return;
	}
	if (s[0] == '=')
	{
		m_Sheet[i][j] = new Expression(s, &m_Sheet);
		return;
	}
	if (s == "-")
	{
		m_Sheet[i][j] = new Empty(&m_Sheet);
		return;
	}
	m_Sheet[i][j] = new Error(s, &m_Sheet);
}

void Spreadsheet::Allocate(const Spreadsheet& obj)
{
	int n = obj.m_Sheet.size();
	if (n != 0)
	{
		int m = obj.m_Sheet[0].size();
		m_Sheet = Matrix<Cell*>(n, std::vector<Cell*>(m));
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				Text* tptr = dynamic_cast<Text*>(obj.m_Sheet[i][j]);
				if (tptr != nullptr)
				{
					m_Sheet[i][j] = new Text(*tptr);
					continue;
				}
				Number* nptr = dynamic_cast<Number*>(obj.m_Sheet[i][j]);
				if (nptr != nullptr)
				{
					m_Sheet[i][j] = new Number(*nptr);
					continue;
				}
				Expression* eptr = dynamic_cast<Expression*>(obj.m_Sheet[i][j]);
				if (eptr != nullptr)
				{
					m_Sheet[i][j] = new Expression(*eptr);
					continue;
				}
				Empty* emptr = dynamic_cast<Empty*>(obj.m_Sheet[i][j]);
				if (emptr != nullptr)
				{
					m_Sheet[i][j] = new Empty(*emptr);
					continue;
				}
				Error* erptr = dynamic_cast<Error*>(obj.m_Sheet[i][j]);
				if (erptr != nullptr)
				{
					m_Sheet[i][j] = new Error(*erptr);
					continue;
				}
			}
		}
	}
}

void Spreadsheet::Deallocate()
{
	int n = m_Sheet.size();
	if (n != 0)
	{
		int m = m_Sheet[0].size();
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				delete m_Sheet[i][j];
				m_Sheet[i][j] = nullptr;
			}
		}
	}
	m_Sheet.clear();
}

std::ostream& operator<<(std::ostream& print, const Spreadsheet& obj)
{
	int n = obj.m_Sheet.size();
	if (n != 0)
	{
		int m = obj.m_Sheet[0].size();
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				print.width(10);
				print << obj.m_Sheet[i][j]->GetResult() << " ";
			}
			print << std::endl;
		}
	}
	return print;
}

void Expression::Calculate()
{
	m_Calculated = true;
	int len = m_data.size();
	std::string now;
	int res = 0;
	char op = '#';
	for (int i = 1; i <= len; ++i)
	{
		if (i == len || isoption(m_data[i]))
		{
			if (iscell(now))
			{
				int jto = now[0] - 'A';
				int ito = now[1] - '1';
				if (!(*m_Sheet)[ito][jto]->IsCalculated())
				{
					(*m_Sheet)[ito][jto]->Calculate();
				}
				if (isnumber((*m_Sheet)[ito][jto]->GetResult()))
				{
					int num = std::stoi((*m_Sheet)[ito][jto]->GetResult());
					if (op == '#')
						res = num;
					else
						res = Option(res, op, num);
				}
				else
				{
					m_result = "#Error";
					return;
				}
			}
			else if (isnumber(now))
			{
				int num = std::stoi(now);
				if (op == '#')
					res = num;
				else
					res = Option(res, op, num);
			}
			else
			{
				m_data = "#Error";
				return;
			}
			now = "";
			if (i != len)
				op = m_data[i];
		}
		else
			now += m_data[i];
	}
	m_result = std::to_string(res);
}