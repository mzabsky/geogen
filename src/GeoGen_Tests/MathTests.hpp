#pragma once

#include "TestFixtureBase.hpp"

class MathTests : public TestFixtureBase
{
public:
	static void TestOverflow()
	{
		TEST_SCRIPT_FAILURE(NumberOverflowException, "\n\
			var a = 1000000000000000000000000000000000000000 * 100000000000000000000000000000000000000 * 1000000000000000000000000000000000 * 1000000000000000000000000000000000 * 1000000000000000000000000000000000 * 1000000000000000000000000000000000 * 1000000000000000000000000000000000 * 1000000000000000000000000000000000* 100000000000000000000000000000000000000 * 1000000000000000000000000000000000 * 1000000000000000000000000000000000 * 1000000000000000000000000000000000 * 1000000000000000000000000000000000 * 1000000000000000000000000000000000 * 1000000000000000000000000000000000;\n\
			Print(\"{0}\", a);\n\
		");
	}

	static void TestUnderflow()
	{
		TEST_SCRIPT_FAILURE(NumberUnderflowException, "\n\
			var a = 0.000000000000000000000000000000000000001 * 0.00000000000000000000000000000000000000000001 * 0.00000000000000000000000000000000000000000001 * 0.00000000000000000000000000000000000000000001 * 0.00000000000000000000000000000000000000000001 * 0.00000000000000000000000000000000000000000001 * 0.00000000000000000000000000000000000000000001 * 0.00000000000000000000000000000000000000000001 * 0.00000000000000000000000000000000000000000001 * 0.00000000000000000000000000000000000000000001 * 0.00000000000000000000000000000000000000000001 * 0.00000000000000000000000000000000000000000001;\n\
		");
	}

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

	static void TestAtan2()
	{
		TestScript("\n\
			var result = Atan2(20, 10);\n\
			var correct = 1.10714;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	static void TestDegToRad()
	{
		TestScript("\n\
			var result = DegToRad(65);\n\
			var correct = 1.1345;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	static void TestRadToDeg()
	{
		TestScript("\n\
			var result = RadToDeg(65);\n\
			var correct = 3724.22566;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	static void TestLog()
	{
		TestScript("\n\
			var result = Log(10, 2);\n\
			var correct = 3.32192;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	static void TestLogFails1()
	{
		TEST_SCRIPT_FAILURE(MathDefinitionRangeException, "\n\
			var result = Log(-10, 2);\n\
		");
	}

	static void TestLogFails2()
	{
		TEST_SCRIPT_FAILURE(MathDefinitionRangeException, "\n\
			var result = Log(10, -2);\n\
		");
	}

	static void TestPow()
	{
		TestScript("\n\
			var result = Pow(10, 2.5);\n\
			var correct = 316.228;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	static void TestSqrt()
	{
		TestScript("\n\
			var result = Sqrt(2);\n\
			var correct = 1.41421;\n\
			AssertEquals(true, result > correct - 0.001);\n\
			AssertEquals(true, result < correct + 0.001);\n\
		");
	}

	static void TestSqrtFails()
	{
		TEST_SCRIPT_FAILURE(MathDefinitionRangeException, "\n\
			var result = Sqrt(-2);\n\
		");
	}

	static void TestRound()
	{
		TestScript("\n\
			AssertEquals(3, Round(2.6));\n\
		");
	}

	static void TestFloor()
	{
		TestScript("\n\
			AssertEquals(2, Floor(2.6));\n\
		");
	}

	static void TestCeil()
	{
		TestScript("\n\
			AssertEquals(3, Ceil(2.6));\n\
		");
	}

	static void TestAbs()
	{
		TestScript("\n\
			AssertEquals(2.6, Abs(-2.6));\n\
		");
	}

	MathTests() : TestFixtureBase("MathTests")
	{
		ADD_TESTCASE(TestOverflow);
		ADD_TESTCASE(TestUnderflow);
		/*ADD_TESTCASE(TestMin);
		ADD_TESTCASE(TestMinTooFewArgs);
		ADD_TESTCASE(TestMax);
		ADD_TESTCASE(TestSin);
		ADD_TESTCASE(TestAsin);
		ADD_TESTCASE(TestAsinFailure);
		ADD_TESTCASE(TestAcos);
		ADD_TESTCASE(TestAcosFailure);
		ADD_TESTCASE(TestAtan);
		ADD_TESTCASE(TestAtan2);
		ADD_TESTCASE(TestDegToRad);
		ADD_TESTCASE(TestRadToDeg);
		ADD_TESTCASE(TestLog);
		ADD_TESTCASE(TestLogFails1);
		ADD_TESTCASE(TestLogFails2);
		ADD_TESTCASE(TestPow);
		ADD_TESTCASE(TestSqrt);
		ADD_TESTCASE(TestSqrtFails);
		ADD_TESTCASE(TestRound);
		ADD_TESTCASE(TestCeil);
		ADD_TESTCASE(TestFloor);
		ADD_TESTCASE(TestAbs);*/
	}
};