#pragma once
#include "Function.h"
#include <unordered_map>
#include <iostream>
#include <memory>


namespace Lisp {

	typedef std::unordered_map<std::string, int> ArgumentMap;


	class Parser {
	public:
		Parser(const std::string&, const ArgumentMap& = ArgumentMap());
	public:
		std::stack<ExecutablePointer> parse();
	private:
		void getDeclaredFucntions();
		std::shared_ptr<Lisp::Function> getFunction(size_t);
	private:
		std::string mExecutableStr;
		const ArgumentMap& mArgMap;
		static std::vector<Function*> builtInFunctions;
		static std::vector<std::shared_ptr<Function>> declaredFunctions;
	};
}

