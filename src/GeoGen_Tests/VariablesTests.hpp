#pragma once

#include "TestFixtureBase.hpp"

class VariablesTests : public TestFixtureBase
{
public:
	static void TestLocalDeclaration()
	{
		TestScript("\n\
			var a = 1; \n\
			\n\
			AssertEquals(1, a);\n\
		");
	}

	static void TestLocalRedefinitionFails()
	{
		TEST_SCRIPT_FAILURE(VariableRedefinitionException, "\n\
			var a = 1; \n\
			var a = 2; \n\
		");
	}

	static void TestNestedLocalRedefinition()
	{
		TestScript("\n\
			var a = 1; \n\
			if(true){ \n\
				var a = 2;\n\
				AssertEquals(2, a);\n\
			} \n\
			\n\
			AssertEquals(1, a);\n\
		");

		// Blok se merguje do rodicovskeho
		TestScript("\n\
			var a = 1; \n\
			{ \n\
				var a = 2;\n\
				AssertEquals(2, a);\n\
			} \n\
			\n\
			AssertEquals(1, a);\n\
		");
	}

	static void TestGlobalDeclaration()
	{
		TestScript("\n\
			global a = 1; \n\
			\n\
			AssertEquals(1, a);\n\
		");
	}

	static void TestGlobalRedefinitionFails()
	{
		TEST_SCRIPT_FAILURE(VariableRedefinitionException, "\n\
			global a = 1; \n\
			global a = 2; \n\
		");
	}
	
	static void TestLocalOverridesGlobal()
	{
		TestScript("\n\
			var a = 1; \n\
			global a = 2; \n\
			\n\
			AssertEquals(1, a); \n\
		");
	}

	static void TestGlobalAfterLocal()
	{
		TestScript("\n\
			global a = 2; \n\
			var a = 1; \n\
			function x() { AssertEquals(2, a); } \n\
			x();\n\
		");
	}

	static void TestLocalIsNotGlobal()
	{
		TEST_SCRIPT_FAILURE(UndefinedSymbolAccessException, "\n\
			var a = 1; \n\
			function x() { a = 2; } \n\
			x();\n\
		");
	}

	VariablesTests() : TestFixtureBase("VariablesTests")
	{
		ADD_TESTCASE(TestLocalDeclaration);
		ADD_TESTCASE(TestLocalRedefinitionFails);
		ADD_TESTCASE(TestNestedLocalRedefinition);
		ADD_TESTCASE(TestGlobalDeclaration);
		ADD_TESTCASE(TestGlobalRedefinitionFails);
		ADD_TESTCASE(TestLocalOverridesGlobal);
		ADD_TESTCASE(TestGlobalAfterLocal);
		ADD_TESTCASE(TestLocalIsNotGlobal);
	}
};