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
			AssertEquals(500, Number.FromCoordinate(@Coordinate.FromNumber(0.5), Direction.Horizontal));\n\
		");
	}

	static void TestRelativeCoordinateFromNumberToNumberDirectionNotSpecifiedFails()
	{
		TEST_SCRIPT_FAILURE(UnknownRelativeCoordinateDirectionException, "\n\
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
			AssertEquals(500, Number.FromCoordinate(@Coordinate.FromNumber(0.5), Direction.Horizontal));\n\
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
			AssertEquals(500, Number.FromCoordinate((@[[0.5], [0.2]]).X, Direction.Horizontal));\n\
			AssertEquals(400, Number.FromCoordinate((@[[0.5], [0.2]]).Y, Direction.Vertical));\n\
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
			AssertEquals(500, Number.FromCoordinate([@[0.5], [200]].X, Direction.Horizontal));\n\
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
			AssertEquals(400, Number.FromCoordinate([[0.5], @[0.2]].Y, Direction.Vertical));\n\
		", params);
	}

	static void TestXRelativePointCreateDirectionNotSpecifiedFails()
	{
		TEST_SCRIPT_FAILURE(UnknownRelativeCoordinateDirectionException, "\n\
			metadata { Width: { Max: 1000, Default: 1000 }, Height: { Max: 2000, Default: 2000 } }\n\
			\n\
			AssertEquals(500, Number.FromCoordinate([@[0.5], [200]].X));\n\
		");
	}

	static void TestYRelativePointCreateDirectionNotSpecifiedFails()
	{
		TEST_SCRIPT_FAILURE(UnknownRelativeCoordinateDirectionException, "\n\
			metadata { Width: { Max: 1000, Default: 1000 }, Height: { Max: 2000, Default: 2000 } }\n\
			\n\
			AssertEquals(400, Number.FromCoordinate([[300], @[0.2]].Y));\n\
		");
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
		ADD_TESTCASE(TestRelativeCoordinateFromNumberToNumberDirectionNotSpecifiedFails);
		ADD_TESTCASE(TestRelativeCoordinateFromNumberToNumberInInfiniteDimensionFails);

		ADD_TESTCASE(TestPointCreate);
		ADD_TESTCASE(TestPointCreateUsingLiteral);
		ADD_TESTCASE(TestBothRelativePointCreate);
		ADD_TESTCASE(TestXRelativePointCreate);
		ADD_TESTCASE(TestYRelativePointCreate);
		ADD_TESTCASE(TestXRelativePointCreateDirectionNotSpecifiedFails);
		ADD_TESTCASE(TestYRelativePointCreateDirectionNotSpecifiedFails);
		ADD_TESTCASE(TestRelativePointCreateInBothInfiniteDimensionsFails);
		ADD_TESTCASE(TestRelativePointCreateInXInfiniteDimensionFails);
		ADD_TESTCASE(TestRelativePointCreateInYInfiniteDimensionFails);
		ADD_TESTCASE(TestConversionFromNumber);
	}
};