#pragma once

#include "TestFixtureBase.hpp"

class ArrayTests : public TestFixtureBase
{
public:
	static void TestCountEmpty()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			\n\
			AssertEquals(0, a.Count());\n\
		");
	}

	ArrayTests() : TestFixtureBase("ArrayTests")
	{
		ADD_TESTCASE(TestCountEmpty);
	}
};