#pragma once

#include "TestFixtureBase.hpp"

class BasicRuntimeTests : public TestFixtureBase
{
public:
	static void TestOperators()
	{
		TestScript("\
			AssertEquals(2, +2);\n\
			AssertEquals(0-2, -2);\n\
			AssertEquals(2, - -2);\n\
			AssertEquals(false, !true);\n\
			AssertEquals(10, 2*5);\n\
			AssertEquals(0, 3*0);\n\
			AssertEquals(-10, 2*-5);\n\
			AssertEquals(2, 20/10);\n\
			AssertEquals(0, 0/10);\n\
		");
	}

	BasicRuntimeTests() : TestFixtureBase("BasicRuntimeTests")
	{
		ADD_TESTCASE(TestOperators);
	}
};