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

	static void TestPushBackIncreasesCount()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a.PushBack(7);\n\
			\n\
			AssertEquals(1, a.Count());\n\
		");
	}

	static void TestPushBackFront()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a.PushBack(1);\n\
			a.PushBack(2);\n\
			\n\
			AssertEquals(1, a.Front());\n\
		");
	}

	static void TestPushBackBack()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a.PushBack(1);\n\
			a.PushBack(2);\n\
			\n\
			AssertEquals(2, a.Back());\n\
		");
	}

	static void TestGetByAutoIndex()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a.PushBack(11);\n\
			a.PushBack(12);\n\
			a.PushBack(13);\n\
			\n\
			AssertEquals(11, a[0]);\n\
			AssertEquals(12, a[1]);\n\
			AssertEquals(13, a[2]);\n\
		");
	}

	static void TestSetGet()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[5] = 20;\n\
			\n\
			AssertEquals(20, a[5]);\n\
		");
	}

	static void TestSetGetString()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			\n\
			AssertEquals(21, a[\"key\"]);\n\
		");
	}

	static void TestContainsKeyReturnsTrue()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			\n\
			AssertEquals(true, a.ContainsKey(\"key\"));\n\
		");
	}

	static void TestContainsKeyReturnsFalse()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			\n\
			AssertEquals(false, a.ContainsKey(\"another\"));\n\
		");
	}

	static void TestContainsValueReturnsTrue()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			\n\
			AssertEquals(true, a.ContainsValue(21);\n\
		");
	}

	static void TestContainsValueReturnsFalse()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			\n\
			AssertEquals(false, a.ContainsValue(29));\n\
		");
	}

	ArrayTests() : TestFixtureBase("ArrayTests")
	{
		ADD_TESTCASE(TestCountEmpty);
		ADD_TESTCASE(TestPushBackIncreasesCount);
		ADD_TESTCASE(TestPushBackFront);
		ADD_TESTCASE(TestPushBackBack);
		ADD_TESTCASE(TestGetByAutoIndex);
		ADD_TESTCASE(TestSetGet);
		ADD_TESTCASE(TestSetGetString);
		ADD_TESTCASE(TestContainsKeyReturnsTrue);
		ADD_TESTCASE(TestContainsKeyReturnsFalse);
		ADD_TESTCASE(TestContainsValueReturnsTrue);
		ADD_TESTCASE(TestContainsValueReturnsFalse);
	}
};