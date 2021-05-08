#include "Function.h"
#include "Parser.h"
#include <sstream>
#include <string>
#include <vector>
#include <stack>



std::vector<Lisp::Function*> Lisp::Parser::builtInFunctions = {
		&Lisp::Add::instance(),
		&Lisp::Sub::instance(),
		&Lisp::Mul::instance(),
		&Lisp::Div::instance(),
		&Lisp::Equal::instance(),
		&Lisp::Lesser::instance(),
		&Lisp::Greater::instance(),
		&Lisp::If::instance(),
		&Lisp::Print::instance(),
};

std::vector<std::shared_ptr<Lisp::Function>> Lisp::Parser::declaredFunctions{};

namespace {
	int stringToInteger(const std::string& s) {
		std::stringstream ss;
		ss << s;
		int res = 0;
		ss >> res;
		return res;
	}
}

Lisp::Parser::Parser(const std::string& s, const Lisp::ArgumentMap& mp)
	:mExecutableStr(s)
	, mArgMap(mp)
{}

std::stack<Lisp::ExecutablePointer> Lisp::Parser::parse() {
	getDeclaredFucntions();

	std::string str;
	std::stack<ExecutablePointer> st;

	const std::string& s = mExecutableStr;
	for (size_t i = s.size() - 1; i < s.size(); --i) {
		if (isdigit(s[i])) {
			str = s[i] + str;
		}
		else {
			if (str != "") {
				int number = stringToInteger(str);
				Executable* ptr = static_cast<Executable*>(new Integer(number));
				st.push(ExecutablePointer(ptr));
				str = "";
			}

			while (i < s.size() && isalpha(s[i])) {
				str = s[i] + str;
				i--;
			}
			auto getExecutable = [&](Function* function) {
				if (str == function->getName()) {
					std::vector<ExecutablePointer> args(function->getArgsCount());
					for (auto& arg : args) {
						arg = st.top(); st.pop();
					}
					Executable* ptr = static_cast<Executable*>(new FunctionExecutable(function, args));
					st.push(ExecutablePointer(ptr));
				}
			};

			for (auto function : builtInFunctions) {
				getExecutable(function);
			}

			for (auto function : declaredFunctions) {
				getExecutable(&*function);
			}

			if (mArgMap.find(str) != mArgMap.end()) {
				int number = mArgMap.find(str)->second;
				Executable* ptr = static_cast<Executable*>(new Integer(number));
				st.push(ExecutablePointer(ptr));
			}
			str = "";
		}
	}

	return st;
}

std::shared_ptr<Lisp::Function> Lisp::Parser::getFunction(size_t start) {
	int brackets = 0;
	size_t end = start;
	for (size_t i = start; i < mExecutableStr.size(); ++i) {
		if (mExecutableStr[i] == '(') {
			brackets++;
		}
		if (mExecutableStr[i] == ')') {
			brackets--;
		}
		if (brackets == 0) {
			end = i + 1;
			break;
		}
	}
	Lisp::Function* res = new Lisp::Function(mExecutableStr.substr(start, end - start));
	mExecutableStr = mExecutableStr.substr(0, start) + mExecutableStr.substr(end, mExecutableStr.size() - end);
	return std::shared_ptr<Lisp::Function>(res);
}

void Lisp::Parser::getDeclaredFucntions() {
	std::string str;
	for (size_t i = 0; i < mExecutableStr.size(); ++i) {
		while (isalpha(mExecutableStr[i])) {
			str += mExecutableStr[i];
			++i;
		}

		if (str == "defun") {
			declaredFunctions.emplace_back(getFunction(i - 6));
			i = i - 7;
		}

		str = "";
	}
}