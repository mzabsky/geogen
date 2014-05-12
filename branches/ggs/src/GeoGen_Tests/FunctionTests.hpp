#pragma once

#include "TestFixtureBase.hpp"
#include "..\GeoGen\runtime\StackOverflowException.hpp"
#include "..\GeoGen\runtime\NumberOfArgumentsException.hpp"

class FunctionTests : public TestFixtureBase
{
public:
	static void TestCallFunction()
	{
		TestScript("\n\
			global a = 1; \n\
			function x(){ a = 2; } \n\
			x(); \n\
			AssertEquals(2, a);\n\
		");
	}

	static void TestNestedCallFunction()
	{
		TestScript("\n\
			global a = 1; \n\
			function x(){ a = 2; } \n\
			function y(){ x(); } \n\
			y(); \n\
			AssertEquals(2, a);\n\
		");
	}

	static void TestReturnValue()
	{
		TestScript("\n\
			var a = 1; \n\
			function x(){ return 2; } \n\
			a = x(); \n\
			AssertEquals(2, a);\n\
		");
	}

	static void TestNestedReturnValue()
	{
		TestScript("\n\
			var a = 1; \n\
			function y(){ return 2; } \n\
			function x(){ return y(); } \n\
			a = x(); \n\
			AssertEquals(2, a);\n\
		");
	}

	static void TestImplicitReturnNull()
	{
		TestScript("\n\
			var a = 1; \n\
			function x(){ var b = 5; } \n\
			a = x(); \n\
			AssertEquals(null, a);\n\
		");
	}

	static void TestUndefinedFunction()
	{
		TEST_SCRIPT_FAILURE(UndefinedSymbolAccessException, "\n\
			x(); \n\
		");
	}

	static void TestFunctionRedefinition()
	{
		TEST_SCRIPT_FAILURE(SymbolRedefinitionException, "\n\
			function x(){ var b = 5; } \n\
			function x(){ var b = 4; } \n\
		");
	}

	static void TestGlobalFunctionRedefinition()
	{
		TEST_SCRIPT_FAILURE(SymbolRedefinitionException, "\n\
			function AssertEquals(){ var b = 5; } \n\
		");
	}

	
	static void TestCallScriptFunctionWithIncorrectNumberOfArguments()
	{
		TEST_SCRIPT_FAILURE(NumberOfArgumentsException, "\n\
			function x(a, b, c){};\n\
			x(1,2);\n\
		");
	}


	static void TestCallStackOverflow()
	{
		TEST_SCRIPT_FAILURE(StackOverflowException, "\n\
			function x(){ x(); } \n\
			x();\n\
		");
	}

	FunctionTests() : TestFixtureBase("FunctionTests")
	{
		ADD_TESTCASE(TestCallFunction);
		ADD_TESTCASE(TestNestedCallFunction);
		ADD_TESTCASE(TestReturnValue);
		ADD_TESTCASE(TestNestedReturnValue);
		ADD_TESTCASE(TestImplicitReturnNull);
		ADD_TESTCASE(TestUndefinedFunction);
		// ADD_TESTCASE(TestGlobalFunctionRedefinition);
		ADD_TESTCASE(TestFunctionRedefinition);
		ADD_TESTCASE(TestCallStackOverflow);
		ADD_TESTCASE(TestCallScriptFunctionWithIncorrectNumberOfArguments);
	}
};