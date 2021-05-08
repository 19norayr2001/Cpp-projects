#pragma once
#include <memory>
#include <string>
#include <vector>
#include <stack>

namespace Lisp {

	class Executable;
	typedef std::shared_ptr<Executable> ExecutablePointer;

	class Function {
	public:
		Function(const std::string & = "");
		virtual int execute(const std::vector<ExecutablePointer>&) const;
		size_t getArgsCount() const { return mArgNames.size(); }
		const std::string& getName() const { return mName; }
	private:
		std::vector<std::string> mArgNames;
		std::string mName;
		std::string mExecutableStr;
	};

	class If : public Function {
	public:
		static If& instance();
		virtual int execute(const std::vector<ExecutablePointer>&) const;
	private:
		If();
	};

	class Add : public Function {
	public:
		static Add& instance();
		virtual int execute(const std::vector<ExecutablePointer>&) const;
	private:
		Add();

	};

	class Sub : public Function {

	public:
		static Sub& instance();
		virtual int execute(const std::vector<ExecutablePointer>&) const;
	private:
		Sub();
	};

	class Mul : public Function {

	public:
		static Mul& instance();
		virtual int execute(const std::vector<ExecutablePointer>&) const;
	private:
		Mul();
	};

	class Div : public Function {

	public:
		static Div& instance();
		virtual int execute(const std::vector<ExecutablePointer>&) const;
	private:
		Div();
	};

	class Lesser : public Function {

	public:
		static Lesser& instance();
		virtual int execute(const std::vector<ExecutablePointer>&) const;
	private:
		Lesser();
	};

	class Greater : public Function {

	public:
		static Greater& instance();
		virtual int execute(const std::vector<ExecutablePointer>&) const;
	private:
		Greater();
	};

	class Equal : public Function {

	public:
		static Equal& instance();
		virtual int execute(const std::vector<ExecutablePointer>&) const;
	private:
		Equal();
	};

	class Print : public Function {
	public:
		static Print& instance();
		virtual int execute(const std::vector<ExecutablePointer>&) const;
	private:
		Print();
	};

	class Executable {
	public:
		virtual int execute() const = 0;
		virtual ~Executable() {}
	};

	class FunctionExecutable : public Executable {
	public:
		FunctionExecutable(Function* function, std::vector<ExecutablePointer> args)
			:mFunction(function)
			, mArgs(args) {}
		int execute() const {
			return mFunction->execute(mArgs);
		}
	private:
		Function* mFunction;
		std::vector<ExecutablePointer> mArgs;
	};

	class Integer : public Executable {
	public:
		Integer(int integer)
			:mInteger(integer) {}

		int execute() const {
			return mInteger;
		}
	private:
		int mInteger;
	};
}

