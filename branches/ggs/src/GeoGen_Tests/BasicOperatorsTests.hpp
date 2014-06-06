#pragma once

#include "TestFixtureBase.hpp"
#include "..\GeoGen\runtime\IncorrectTypeException.hpp"
#include "..\GeoGen\corelib\DivisionByZeroException.hpp"

class BasicOperatorsTests : public TestFixtureBase
{
public:
	static void TestOperators()
	{
		TestScript("\
			AssertEquals(2, +2);\n\
			AssertEquals(0-2, -2);\n\
			AssertEquals(2, - -2);\n\
			AssertEquals(0.5, - -0.5);\n\
			AssertEquals(false, !true);\n\
			AssertEquals(10, 2*5);\n\
			AssertEquals(0, 3*0);\n\
			AssertEquals(-10, 2*-5);\n\
			AssertEquals(2, 20/10);\n\
			AssertEquals(2.5, 5/2);\n\
			AssertEquals(2, 0.5*4);\n\
			AssertEquals(0, 0/10);\n\
			AssertEquals(3, 8%5);\n\
			AssertEquals(0, 0%5);\n\
			AssertEquals(9, 4+5);\n\
			AssertEquals(1.5, 1+0.5);\n\
			AssertEquals(3, -2+5);\n\
			AssertEquals(8, 9-1);\n\
			AssertEquals(4, 3 - -1);\n\
			AssertEquals(1.5, 2 - 0.5);\n\
			AssertEquals(8, 2 << 2);\n\
			AssertEquals(8, 2.6 << 2);\n\
			AssertEquals(8, 2.6 << 2.6);\n\
			AssertEquals(2, 8 >> 2);\n\
			AssertEquals(2, 8 >> 2);\n\
			AssertEquals(2, 8.5 >> 2);\n\
			AssertEquals(2, 8.5 >> 2.6);\n\
			AssertEquals(0, 2 >> 3);\n\
			AssertEquals(0, 2 >> 3);\n\
			AssertEquals(true, 1 < 2);\n\
			AssertEquals(false, 1 < 1);\n\
			AssertEquals(false, 2 < 1);\n\
			AssertEquals(false, 1 > 2);\n\
			AssertEquals(false, 1 > 1);\n\
			AssertEquals(true, 2 > 1);\n\
			AssertEquals(true, 1 <= 2);\n\
			AssertEquals(true, 1 <= 1);\n\
			AssertEquals(false, 2 <= 1);\n\
			AssertEquals(false, 1 >= 2);\n\
			AssertEquals(true, 1 >= 1);\n\
			AssertEquals(true, 2 >= 1);\n\
			AssertEquals(true, 1 == 1);\n\
			AssertEquals(true, 0.5 == 0.5);\n\
			AssertEquals(true, true == true);\n\
			AssertEquals(true, false == false);\n\
			AssertEquals(true, null == null);\n\
			AssertEquals(false, 2 == 0.5);\n\
			AssertEquals(false, true == false);\n\
			AssertEquals(false, true == 1);\n\
			AssertEquals(false, 1 == true);\n\
			AssertEquals(false, false == 0);\n\
			AssertEquals(false, 0 == false);\n\
			AssertEquals(false, 2 == null);\n\
			AssertEquals(false, null == 2);\n\
			AssertEquals(false, true == null);\n\
			AssertEquals(false, null == true);\n\
			AssertEquals(false, false == null);\n\
			AssertEquals(false, null == false);\n\
			AssertEquals(true, 2 != 0.5); \n\
			AssertEquals(true, true != false); \n\
			AssertEquals(true, true != 1); \n\
			AssertEquals(true, 1 != true); \n\
			AssertEquals(true, false != 0); \n\
			AssertEquals(true, 0 != false); \n\
			AssertEquals(true, 2 != null); \n\
			AssertEquals(true, null != 2); \n\
			AssertEquals(true, true != null); \n\
			AssertEquals(true, null != true); \n\
			AssertEquals(true, false != null); \n\
			AssertEquals(true, null != false); \n\
			AssertEquals(0, 7 & 0); \n\
			AssertEquals(2, 7 & 2); \n\
			AssertEquals(5, 7 & 13); \n\
			AssertEquals(7, 7 & 7); \n\
			AssertEquals(7, 7.5 & 7); \n\
			AssertEquals(7, 7.5 & 7.5); \n\
			AssertEquals(7, 7 & 7.5); \n\
			AssertEquals(0, 2 & true); \n\
			AssertEquals(1, 1 & true); \n\
			AssertEquals(0, 1 & false); \n\
			AssertEquals(0, true & 2); \n\
			AssertEquals(1, true & 1); \n\
			AssertEquals(0, false & 1); \n\
			AssertEquals(false, false & true); \n\
			AssertEquals(false, true & false); \n\
			AssertEquals(true, true & true); \n\
			AssertEquals(false, false & false); \n\
			AssertEquals(7, 7 | 0); \n\
			AssertEquals(3, 2 | 1); \n\
			AssertEquals(3, 3 | 3); \n\
			AssertEquals(1, 0 | 1); \n\
			AssertEquals(3, 2 | 1.5); \n\
			AssertEquals(3, 2.2 | 1); \n\
			AssertEquals(3, 2.2 | 1.5); \n\
			AssertEquals(3, 2 | true); \n\
			AssertEquals(2, 2 | false); \n\
			AssertEquals(3, true | 2); \n\
			AssertEquals(2, false | 2); \n\
			AssertEquals(true, false | true); \n\
			AssertEquals(true, true | false); \n\
			AssertEquals(true, true | true); \n\
			AssertEquals(false, false | false); \n\
			AssertEquals(7, 7 ^ 0); \n\
			AssertEquals(3, 2 ^ 1); \n\
			AssertEquals(0, 3 ^ 3); \n\
			AssertEquals(1, 0 ^ 1); \n\
			AssertEquals(5, 6 ^ 3); \n\
			AssertEquals(3, 2 ^ 1.5); \n\
			AssertEquals(3, 2.2 ^ 1); \n\
			AssertEquals(3, 2.2 ^ 1.5); \n\
			AssertEquals(0, 2.2 ^ 2.2); \n\
			AssertEquals(3, 2 ^ true); \n\
			AssertEquals(2, 3 ^ true); \n\
			AssertEquals(2, 2 ^ false); \n\
			AssertEquals(3, true ^ 2); \n\
			AssertEquals(2, false ^ 2); \n\
			AssertEquals(3, false ^ 3); \n\
			AssertEquals(true, false ^ true); \n\
			AssertEquals(true, true ^ false); \n\
			AssertEquals(false, true ^ true); \n\
			AssertEquals(false, false ^ false); \n\
			AssertEquals(true, true && true); \n\
			AssertEquals(false, false && true); \n\
			AssertEquals(false, true && false); \n\
			AssertEquals(false, false && false); \n\
			AssertEquals(true, true || true); \n\
			AssertEquals(true, false || true); \n\
			AssertEquals(true, true || false); \n\
			AssertEquals(false, false || false); \n\
		");
	}

	static void TestOperatorFailures()
	{
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "+true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "+null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "+Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "-true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "-null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "-Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "!2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "!null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "!Boolean;");

		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 * true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true * 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 * null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null * 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 * Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean * 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 / true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true / 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 / null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null / 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 / Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean / 2;");
		TEST_SCRIPT_FAILURE(DivisionByZeroException, "2 / 0;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 % true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true % 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 % null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null % 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 % Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean % 2;");
		TEST_SCRIPT_FAILURE(DivisionByZeroException, "2 % 0;");

		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 + true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true + 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 + null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null + 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 + Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean + 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 - true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true - 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 - null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null - 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 - Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean - 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 << true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true << 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 << null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null << 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 << Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean << 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 >> true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true >> 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 >> null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null >> 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 >> Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean >> 2;");

		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 < true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true < 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 < null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null < 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 < Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean < 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 <= true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true <= 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 <= null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null <= 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 <= Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean <- 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 > true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true > 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 > null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null > 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 > Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean > 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 >= true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true >= 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 >= null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null >= 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 >= Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean >= 2;");

		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 & null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null & 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 & Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean & 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 ^ null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null ^ 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 ^ Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean ^ 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 | null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null | 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 | Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean | 2;");

		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 && true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true && 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true && null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null && true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true && Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean && true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "2 || true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true || 2;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true || null;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "null || true;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "true || Boolean;");
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "Boolean || true;");
	}

	static void TestConditionalOperatorTrue()
	{
		TestScript("\
			AssertEquals(1, true ? 1 : 2);\n\
		");
	}

	static void TestConditionalOperatorFalse()
	{
		TestScript("\
			AssertEquals(2, false ? 1 : 2);\n\
		");
	}

	BasicOperatorsTests() : TestFixtureBase("BasicOperatorsTests")
	{
		ADD_TESTCASE(TestOperators);
		ADD_TESTCASE(TestOperatorFailures);
		ADD_TESTCASE(TestConditionalOperatorTrue);
		ADD_TESTCASE(TestConditionalOperatorFalse);
	}
};