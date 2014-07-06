#pragma once

#include "TestFixtureBase.hpp"
#include "../GeoGen/corelib/UnknownRelativeCoordinateOrientationException.hpp"
#include "../GeoGen/corelib/RelativeCoordinateInInfiniteDimensionException.hpp"

class CoordinateTests : public TestFixtureBase
{
public:
	static void TestCoordinateFromNumberToNumber()
	{
		TestScript("\n\
			AssertEquals(100, Number.FromCoordinate(Coordinate.FromNumber(100)));\n\
		");
	}

	static void TestRelativeCoordinateFromNumberToNumber()
	{
		TestScript("\n\
			metadata { Width: { Max: 1000, Default: 1000 } }\n\
			\n\
			AssertEquals(500, Number.FromCoordinate(@Coordinate.FromNumber(0.5), Orientation.Horizontal));\n\
		");
	}

	static void TestRelativeCoordinateFromNumberToNumberOrientationNotSpecifiedFails()
	{
		TEST_SCRIPT_FAILURE(UnknownRelativeCoordinateOrientationException, "\n\
			metadata { Width: { Max: 1000, Default: 1000 } }\n\
			\n\
			AssertEquals(500, Number.FromCoordinate(@Coordinate.FromNumber(0.5)));\n\
		");
	}

	static void TestRelativeCoordinateFromNumberToNumberInInfiniteDimensionFails()
	{
		TEST_SCRIPT_FAILURE(RelativeCoordinateInInfiniteDimensionException, "\n\
			metadata { Width: Infinite }\n\
			\n\
			AssertEquals(500, Number.FromCoordinate(@Coordinate.FromNumber(0.5), Orientation.Horizontal));\n\
		");
	}

	CoordinateTests() : TestFixtureBase("CoordinateTests")
	{
		ADD_TESTCASE(TestCoordinateFromNumberToNumber);
		ADD_TESTCASE(TestRelativeCoordinateFromNumberToNumber);
		ADD_TESTCASE(TestRelativeCoordinateFromNumberToNumberOrientationNotSpecifiedFails);
		ADD_TESTCASE(TestRelativeCoordinateFromNumberToNumberInInfiniteDimensionFails);
	}
};