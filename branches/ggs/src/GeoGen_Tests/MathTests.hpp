#pragma once

#include "TestFixtureBase.hpp"
#include "..\GeoGen\runtime\NumberOfArgumentsException.hpp"
#include "..\GeoGen\corelib\MathDefinitionRangeException.hpp"

class MathTests : public TestFixtureBase
{
public:
	static void TestMin()
	{
		TestScript("\n\
			AssertEquals(5, Min(13, 8, 5, 22));\n\
		");
	}

	static void TestMinTooFewArgs()
	{
		TEST_SCRIPT_FAILURE(NumberOfArgumentsException, "\n\
			Min(13);\n\
		");
	}

	static void TestMax()
	{
		TestScript("\n\
			AssertEquals(22, Max(13, 8, 22, 5));\n\
		");
	}

	static void TestSin()
	{
		TestScript("\n\
			var result = Sin(8);\n\
			var correct = 0.98935;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	static void TestCos()
	{
		TestScript("\n\
			var result = Cos(8);\n\
			var correct = -0.14550;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	static void TestTan()
	{
		TestScript("\n\
			var result = Cos(8);\n\
			var correct = -6.79971;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	static void TestAsin()
	{
		TestScript("\n\
			var result = Asin(0.5);\n\
			var correct = 0.52359;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	static void TestAsinFailure()
	{
		TEST_SCRIPT_FAILURE(MathDefinitionRangeException, "\n\
			Asin(2);\n\
		");
	}

	static void TestAcos()
	{
		TestScript("\n\
			var result = Acos(0.5);\n\
			var correct = 1.04720;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	static void TestAcosFailure()
	{
		TEST_SCRIPT_FAILURE(MathDefinitionRangeException, "\n\
			Acos(2);\n\
		");
	}

	static void TestAtan()
	{
		TestScript("\n\
			var result = Atan(8);\n\
			var correct = 1.44644;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	MathTests() : TestFixtureBase("MathTests")
	{
		ADD_TESTCASE(TestMin);
		ADD_TESTCASE(TestMinTooFewArgs);
		ADD_TESTCASE(TestMax);
		ADD_TESTCASE(TestSin);
		ADD_TESTCASE(TestAsin);
		ADD_TESTCASE(TestAsinFailure);
		ADD_TESTCASE(TestAcos);
		ADD_TESTCASE(TestAcosFailure);
		ADD_TESTCASE(TestAtan);
	}
};