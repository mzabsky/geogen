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

	static void TestCoordinateFromNumberToNumberUsingLiteral()
	{
		TestScript("\n\
			AssertEquals(100, Number.FromCoordinate([100]));\n\
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

	/// Initializes a new instance of the CoordinateTests class./////////////////////////////////////////
	

	static void TestPointCreate()
	{
		TestScript("\n\
			AssertEquals(100, Number.FromCoordinate(Point.Create([100], [200]).X));\n\
			AssertEquals(200, Number.FromCoordinate(Point.Create([100], [200]).Y));\n\
		");
	}

	static void TestPointCreateUsingLiteral()
	{
		TestScript("\n\
			AssertEquals(100, Number.FromCoordinate([[100], [200]].X));\n\
			AssertEquals(200, Number.FromCoordinate([[100], [200]].Y));\n\
		");
	}

	static void TestBothRelativePointCreate()
	{
		ScriptParameters params;
		params.SetMapWidth(1000);
		params.SetMapHeight(2000);

		TestScript("\n\
			metadata { Width: { Max: 1000, Default: 1000 }, Height: { Max: 2000, Default: 2000 } }\n\
			\n\
			AssertEquals(500, Number.FromCoordinate((@[[0.5], [0.2]]).X, Orientation.Horizontal));\n\
			AssertEquals(400, Number.FromCoordinate((@[[0.5], [0.2]]).Y, Orientation.Vertical));\n\
		", params);
	}

	static void TestXRelativePointCreate()
	{
		ScriptParameters params;
		params.SetMapWidth(1000);
		params.SetMapHeight(2000);

		TestScript("\n\
			metadata { Width: { Max: 1000, Default: 1000 }, Height: { Max: 2000, Default: 2000 } }\n\
			\n\
			AssertEquals(500, Number.FromCoordinate([@[0.5], [200]].X, Orientation.Horizontal));\n\
			AssertEquals(200, Number.FromCoordinate([@[0.5], [200]].Y));\n\
		", params);
	}

	static void TestYRelativePointCreate()
	{
		ScriptParameters params;
		params.SetMapWidth(1000);
		params.SetMapHeight(2000);

		TestScript("\n\
			metadata { Width: { Max: 1000, Default: 1000 }, Height: { Max: 2000, Default: 2000 } }\n\
			\n\
			AssertEquals(300, Number.FromCoordinate([[300], @[0.2]].X));\n\
			AssertEquals(400, Number.FromCoordinate([[0.5], @[0.2]].Y, Orientation.Vertical));\n\
		", params);
	}

	static void TestXRelativePointCreateOrientationNotSpecifiedFails()
	{
		ScriptParameters params;
		params.SetMapWidth(1000);
		params.SetMapHeight(2000);

		TEST_SCRIPT_FAILURE(UnknownRelativeCoordinateOrientationException, "\n\
			metadata { Width: { Max: 1000, Default: 1000 }, Height: { Max: 2000, Default: 2000 } }\n\
			\n\
			AssertEquals(500, Number.FromCoordinate([@[0.5], [200]].X));\n\
		", params);
	}

	static void TestYRelativePointCreateOrientationNotSpecifiedFails()
	{
		ScriptParameters params;
		params.SetMapWidth(1000);
		params.SetMapHeight(2000);

		TEST_SCRIPT_FAILURE(UnknownRelativeCoordinateOrientationException, "\n\
			metadata { Width: { Max: 1000, Default: 1000 }, Height: { Max: 2000, Default: 2000 } }\n\
			\n\
			AssertEquals(400, Number.FromCoordinate([[300], @[0.2]].Y));\n\
		", params);
	}

	static void TestRelativePointCreateInBothInfiniteDimensionsFails()
	{
		TEST_SCRIPT_FAILURE(RelativeCoordinateInInfiniteDimensionException, "\n\
			metadata { Width: Infinite, Height: Infinite }\n\
			\n\
			@[[0.5], [0.2]];\n\
		");
	}


	static void TestRelativePointCreateInXInfiniteDimensionFails()
	{
		TEST_SCRIPT_FAILURE(RelativeCoordinateInInfiniteDimensionException, "\n\
			metadata { Width: Infinite, Height: { Max: 2000, Default: 2000 } }\n\
			\n\
			[@[0.5], [200]];\n\
		");
	}

	static void TestRelativePointCreateInYInfiniteDimensionFails()
	{
		TEST_SCRIPT_FAILURE(RelativeCoordinateInInfiniteDimensionException, "\n\
			metadata { Width: { Max: 1000, Default: 1000 }, Height: Infinite }\n\
			\n\
			[[200], @[0.2]];\n\
		");
	}

	static void TestConversionFromNumber()
	{
		TestScript("\n\
			AssertEquals(100, Number.FromCoordinate(100));\n\
		");
	}

	CoordinateTests() : TestFixtureBase("CoordinateTests")
	{
		ADD_TESTCASE(TestCoordinateFromNumberToNumber);
		ADD_TESTCASE(TestCoordinateFromNumberToNumberUsingLiteral);
		ADD_TESTCASE(TestRelativeCoordinateFromNumberToNumber);
		ADD_TESTCASE(TestRelativeCoordinateFromNumberToNumberOrientationNotSpecifiedFails);
		ADD_TESTCASE(TestRelativeCoordinateFromNumberToNumberInInfiniteDimensionFails);

		ADD_TESTCASE(TestPointCreate);
		ADD_TESTCASE(TestPointCreateUsingLiteral);
		ADD_TESTCASE(TestBothRelativePointCreate);
		ADD_TESTCASE(TestXRelativePointCreate);
		ADD_TESTCASE(TestYRelativePointCreate);
		ADD_TESTCASE(TestXRelativePointCreateOrientationNotSpecifiedFails);
		ADD_TESTCASE(TestYRelativePointCreateOrientationNotSpecifiedFails);
		ADD_TESTCASE(TestRelativePointCreateInBothInfiniteDimensionsFails);
		ADD_TESTCASE(TestRelativePointCreateInXInfiniteDimensionFails);
		ADD_TESTCASE(TestRelativePointCreateInYInfiniteDimensionFails);
		ADD_TESTCASE(TestConversionFromNumber);
	}
};