#include "Parser.h"
#include <iostream>
#include <string>

int main() {
	std::string s, t;
	std::cin >> s;
	while (std::cin >> t)
	{
		s += " " + t;
	}

	auto st = Lisp::Parser(s).parse();
	while (!st.empty()) {
		auto executable = st.top(); st.pop();
		executable->execute();
	}
	return 0;
}