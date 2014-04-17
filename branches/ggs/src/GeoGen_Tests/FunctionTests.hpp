#pragma once

#include "TestFixtureBase.hpp"
#include "..\GeoGen\runtime\StackOverflowException.hpp"

class FunctionTests : public TestFixtureBase
{
public:
	static void TestCallStackOverflow()
	{
		TEST_SCRIPT_FAILURE(StackOverflowException, "\n\
			function x(){ x(); } \n\
			x();\n\
		");
	}

	FunctionTests() : TestFixtureBase("FunctionTests")
	{
		ADD_TESTCASE(TestCallStackOverflow);
	}
};