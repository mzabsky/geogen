#pragma once

#include "TestFixtureBase.hpp"

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
			var a = \"aaaaa\";\n\
			AssertEquals(2, false ? 1 : 2);\n\
		");
	}

	static void TestPlusAssignmentOperator()
	{
		TestScript("\
			var a = 5;\n\
			AssertEquals(9, a += 4); \n\
			AssertEquals(a, 9);\n\
		");
	}

	static void TestSubtractionAssignmentOperator()
	{
		TestScript("\
			var a = 5;\n\
			AssertEquals(1, a -= 4); \n\
			AssertEquals(a, 1);\n\
		");
	}

	static void TestMultiplicationAssignmentOperator()
	{
		TestScript("\
			var a = 5;\n\
			AssertEquals(20, a *= 4); \n\
			AssertEquals(a, 20);\n\
		");
	}

	static void TestDivisionAssignmentOperator()
	{
		TestScript("\
			var a = 6;\n\
			AssertEquals(1.5, a /= 4); \n\
			AssertEquals(a, 1.5);\n\
		");
	}

	static void TestDivisionByZeroAssignmentOperatorFails()
	{
		TEST_SCRIPT_FAILURE(DivisionByZeroException, "\
			var a = 6;\n\
			a /= 0;\n\
		");
	}

	static void TestModuloAssignmentOperator()
	{
		TestScript("\
			var a = 5;\n\
			AssertEquals(1, a %= 4); \n\
			AssertEquals(a, 1);\n\
		");
	}

	static void TestModuloByZeroAssignmentOperatorFails()
	{
		TEST_SCRIPT_FAILURE(DivisionByZeroException, "\
			var a = 6;\n\
			a %= 0;\n\
		");
	}

	static void TestShiftLeftAssignmentOperator()
	{
		TestScript("\
			var a = 5;\n\
			AssertEquals(20, a <<= 2); \n\
			AssertEquals(a, 20);\n\
		");
	}

	static void TestShiftRightAssignmentOperator()
	{
		TestScript("\
			var a = 20;\n\
			AssertEquals(5, a >>= 2); \n\
			AssertEquals(a, 5);\n\
		");
	}

	static void TestBitAndAssignmentWithBoolsOperator()
	{
		TestScript("\
			var a = true;\n\
			AssertEquals(false, a &= false); \n\
			AssertEquals(a, false);\n\
		");
	}

	static void TestBitAndAssignmentWithNumbersOperator()
	{
		TestScript("\
			var a = 6;\n\
			AssertEquals(2, a &= 3); \n\
			AssertEquals(a, 2);\n\
		");
	}

	static void TestBitAndAssignmentWithNumberAndBoolOperatorFails()
	{
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "\
			var a = 6;\n\
			a &= true; \n\
		");
	}


	static void TestBitOrAssignmentWithBoolsOperator()
	{
		TestScript("\
			var a = false;\n\
			AssertEquals(true, a |= true); \n\
			AssertEquals(a, true);\n\
		");
	}

	static void TestBitOrAssignmentWithNumbersOperator()
	{
		TestScript("\
			var a = 6;\n\
			AssertEquals(7, a |= 3); \n\
			AssertEquals(a, 7);\n\
		");
	}

	static void TestBitOrAssignmentWithNumberAndBoolOperatorFails()
	{
		TEST_SCRIPT_FAILURE(IncorrectTypeException, "\
			var a = 6;\n\
			a |= true; \n\
		");
	}

	static void TestPreIncrement()
	{
		TestScript("\
			var a = 6;\n\
			AssertEquals(7, ++a); \n\
			AssertEquals(a, 7);\n\
		");
	}

	static void TestPostIncrement()
	{
		TestScript("\
			var a = 6;\n\
			AssertEquals(6, a++); \n\
			AssertEquals(a, 7);\n\
		");
	}

	static void TestPreDecrement()
	{
		TestScript("\
			var a = 6;\n\
			AssertEquals(5, --a); \n\
			AssertEquals(a, 5);\n\
		");
	}

	static void TestPostDecrement()
	{
		TestScript("\
			var a = 6;\n\
			AssertEquals(6, a--); \n\
			AssertEquals(a, 5);\n\
		");
	}

	BasicOperatorsTests() : TestFixtureBase("BasicOperatorsTests")
	{
		ADD_TESTCASE(TestOperators);
	    ADD_TESTCASE(TestOperatorFailures);
		ADD_TESTCASE(TestConditionalOperatorTrue);
		ADD_TESTCASE(TestConditionalOperatorFalse);

		ADD_TESTCASE(TestPlusAssignmentOperator);
		ADD_TESTCASE(TestSubtractionAssignmentOperator);
		ADD_TESTCASE(TestMultiplicationAssignmentOperator);
		ADD_TESTCASE(TestDivisionAssignmentOperator);
		ADD_TESTCASE(TestDivisionByZeroAssignmentOperatorFails);
		ADD_TESTCASE(TestModuloAssignmentOperator);
		ADD_TESTCASE(TestModuloByZeroAssignmentOperatorFails);
		ADD_TESTCASE(TestShiftLeftAssignmentOperator);
		ADD_TESTCASE(TestShiftRightAssignmentOperator);
		ADD_TESTCASE(TestBitAndAssignmentWithBoolsOperator);
		ADD_TESTCASE(TestBitAndAssignmentWithNumbersOperator);
		ADD_TESTCASE(TestBitAndAssignmentWithNumberAndBoolOperatorFails);
		ADD_TESTCASE(TestBitOrAssignmentWithBoolsOperator);
		ADD_TESTCASE(TestBitOrAssignmentWithNumbersOperator);
		ADD_TESTCASE(TestBitOrAssignmentWithNumberAndBoolOperatorFails);

		ADD_TESTCASE(TestPreIncrement);
		ADD_TESTCASE(TestPostIncrement);
		ADD_TESTCASE(TestPreDecrement);
		ADD_TESTCASE(TestPostDecrement);
	}
};