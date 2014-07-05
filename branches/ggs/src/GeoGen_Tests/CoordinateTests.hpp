#pragma once

#include "TestFixtureBase.hpp"

class CoordinateTests : public TestFixtureBase
{
public:
	static void TestCoordinateFromNumberToNumber()
	{
		TestScript("\n\
			AssertEquals(100, Number.FromCoordinate(Coordinate.FromNumber(100)));\n\
		");
	}

	CoordinateTests() : TestFixtureBase("CoordinateTests")
	{
		ADD_TESTCASE(TestCoordinateFromNumberToNumber);
	}
};