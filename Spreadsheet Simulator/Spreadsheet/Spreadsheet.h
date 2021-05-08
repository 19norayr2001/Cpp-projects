#pragma once
#include <iostream>
#include <vector>
class Cell
{
public:
	template<typename Type>
	using Matrix = std::vector<std::vector<Type>>;

public:
	Cell(const std::string& data, Matrix<Cell*>* sheet = nullptr)
		: m_Sheet(sheet)
		, m_data(data)
		, m_Calculated(false)
	{}
	virtual void Calculate() = 0;
	const std::string& GetResult() const { return m_result; }
	bool IsCalculated() const { return m_Calculated; }
	virtual ~Cell() {};
protected:
	Matrix<Cell*>* m_Sheet;
	std::string m_data;
	std::string m_result;
	bool m_Calculated;
};
class Text :public Cell
{
public:
	Text(const std::string& data, Matrix<Cell*>* sheet = nullptr)
		:Cell(data, sheet)
	{}
	void Calculate(){ m_result = m_data.substr(1, m_data.size() - 1); }
};
class Number :public Cell
{
public:
	Number(const std::string& data, Matrix<Cell*>* sheet = nullptr)
		:Cell(data, sheet)
	{}
	void Calculate() { m_result = m_data; }
};
class Expression :public Cell
{
public:
	Expression(const std::string& data, Matrix<Cell*>* sheet = nullptr)
		:Cell(data, sheet)
	{}
	void Calculate();
};
class Error :public Cell
{
public:
	Error(const std::string& data, Matrix<Cell*>* sheet = nullptr)
		:Cell(data, sheet)
	{}
	void Calculate() { m_result = "#Error"; }
};
class Empty :public Cell
{
public:
	Empty(Matrix<Cell*>* sheet = nullptr)
		:Cell("", sheet)
	{}
	void Calculate() {}
};
class Spreadsheet
{
	template<typename Type>
	using Matrix = std::vector<std::vector<Type>>;


	friend std::ostream& operator<<(std::ostream&, const Spreadsheet&);
public:
	Spreadsheet();
	Spreadsheet(const Spreadsheet&);
	Spreadsheet& operator=(const Spreadsheet&);
	~Spreadsheet();
public:
	void NewSpreadsheet(int, int, const Matrix<std::string>&);
private:
	void Parsing(int, int, const std::string&);
private:
	void Allocate(const Spreadsheet&);
	void Deallocate();
private:
	Matrix<Cell*> m_Sheet;
};

