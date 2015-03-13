/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include "TestFixtureBase.hpp"

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

	static void TestArgumentOrder()
	{
		TestScript("\n\
			function x(a, b, c){ \n\
				AssertEquals(1, a);\n\
				AssertEquals(2, b);\n\
				AssertEquals(3, c);\n\
			} \n\
			var i = 1;\n\
			x(i++, i++, i++);\n\
			AssertEquals(4, i);\n\
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
		ADD_TESTCASE(TestArgumentOrder);
	}
};