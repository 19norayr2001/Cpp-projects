#pragma once
class EilerFunction {
public:
private:
	static EilerFunction instance;
	EilerFunction() {}
public:
	static const EilerFunction& getInstance();
public:
	int calculate(int) const;
};

