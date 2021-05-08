#include "Function.h"
#include "Parser.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>


Lisp::Function::Function(const std::string& s) {
	size_t indStart = 0, indEnd = 0;
	for (size_t i = 1; i < s.size(); ++i) {
		if (s[i] == '(') {
			indStart = i;
			continue;
		}

		if (s[i] == ')') {
			indEnd = i;
			break;
		}
	}

	for (size_t i = indStart - 1; !isspace(s[i]); --i) {
		mName = s[i] + mName;
	}

	std::stringstream argsStringStream;
	argsStringStream << s.substr(indStart + 1, indEnd - indStart - 1);

	std::string arg;
	while (argsStringStream >> arg) {
		mArgNames.push_back(arg);
	}

	mExecutableStr = s.substr(indEnd + 1, s.size() - indEnd - 2);
}

int Lisp::Function::execute(const std::vector<ExecutablePointer>& args) const {
	ArgumentMap argMap;
	for (size_t i = 0; i < args.size(); ++i) {
		argMap[mArgNames[i]] = args[i]->execute();
	}
	std::stack<ExecutablePointer> st = Parser(mExecutableStr, argMap).parse();
	while (st.size() > 1) {
		ExecutablePointer executable = st.top(); st.pop();
		executable->execute();
	}
	return st.top()->execute();
	return 0;
}

Lisp::If& Lisp::If::instance() {
	static If instance;
	return instance;
}

Lisp::If::If()
	:Function("(defun if(a b c))") {
}

int Lisp::If::execute(const std::vector<ExecutablePointer>& args)  const {
	return (args[0]->execute() ? args[1]->execute() : args[2]->execute());
}

Lisp::Add& Lisp::Add::instance() {
	static Add instance;
	return instance;
}

Lisp::Add::Add()
	:Function("(defun add(a b))") {
}

int Lisp::Add::execute(const std::vector<ExecutablePointer>& args) const {
	return args[0]->execute() + args[1]->execute();
}


Lisp::Sub& Lisp::Sub::instance() {
	static Sub instance;
	return instance;
}

Lisp::Sub::Sub()
	:Function("(defun sub(a b))") {
}

int Lisp::Sub::execute(const std::vector<ExecutablePointer>& args) const {
	return args[0]->execute() - args[1]->execute();
}

Lisp::Mul& Lisp::Mul::instance() {
	static Mul instance;
	return instance;
}

Lisp::Mul::Mul()
	:Function("(defun mul(a b))") {
}

int Lisp::Mul::execute(const std::vector<ExecutablePointer>& args) const {
	return args[0]->execute() * args[1]->execute();
}

Lisp::Div& Lisp::Div::instance() {
	static Div instance;
	return instance;
}

Lisp::Div::Div()
	:Function("(defun div(a b))") {
}

int Lisp::Div::execute(const std::vector<ExecutablePointer>& args) const {
	return args[0]->execute() / args[1]->execute();
}

Lisp::Lesser& Lisp::Lesser::instance() {
	static Lesser instance;
	return instance;
}

Lisp::Lesser::Lesser()
	:Function("(defun lt(a b))") {
}

int Lisp::Lesser::execute(const std::vector<ExecutablePointer>& args) const {
	return (args[0]->execute() < args[1]->execute());
}

Lisp::Greater& Lisp::Greater::instance() {
	static Greater instance;
	return instance;
}

Lisp::Greater::Greater()
	:Function("(defun gt(a b))") {
}

int Lisp::Greater::execute(const std::vector<ExecutablePointer>& args) const {
	return (args[0]->execute() > args[1]->execute());
}

Lisp::Equal& Lisp::Equal::instance() {
	static Equal instance;
	return instance;
}

Lisp::Equal::Equal()
	:Function("(defun eq(a b))") {
}

int Lisp::Equal::execute(const std::vector<ExecutablePointer>& args) const {
	return (args[0]->execute() == args[1]->execute());
}

Lisp::Print& Lisp::Print::instance() {
	static Print instance;
	return instance;
}

Lisp::Print::Print()
	:Function("(defun print(a))") {
}

int Lisp::Print::execute(const std::vector<ExecutablePointer>& args) const {
	int arg = args[0]->execute();
	std::cout << arg << std::endl;
	return arg;
}