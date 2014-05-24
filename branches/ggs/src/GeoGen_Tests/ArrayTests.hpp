#pragma once

#include "TestFixtureBase.hpp"
#include "..\GeoGen\corelib\ArrayKeyNotFoundException.hpp"

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

	static void TestIndexerPassThrough()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			\n\
			AssertEquals(20, a[5] = 20);\n\
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
			AssertEquals(true, a.ContainsValue(21));\n\
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

	static void TestRemoveKeyRemoves()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			a[\"key2\"] = 22;\n\
			a.RemoveKey(\"key\");\n\
			\n\
			AssertEquals(false, a.ContainsKey(\"key\"));\n\
		");
	}

	static void TestRemoveKeyThrowsIfNotFound()
	{
		TEST_SCRIPT_FAILURE(ArrayKeyNotFoundException, "\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			a[\"key2\"] = 22;\n\
			a.RemoveKey(\"key5\");\n\
		");
	}

	static void TestRemoveValueRemoves()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			a[\"key2\"] = 22;\n\
			a.RemoveValue(21);\n\
			\n\
			AssertEquals(false, a.ContainsKey(\"key\"));\n\
		");
	}

	static void TestRemoveValueRemovesOnlyFirst()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			a[\"key2\"] = 21;\n\
			a.RemoveValue(21);\n\
			\n\
			AssertEquals(true, a.ContainsKey(\"key2\"));\n\
		");
	}

	static void TestRemoveValueDoesNothingIfNotFound()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			a[\"key2\"] = 22;\n\
			a.RemoveValue(25);\n\
			\n\
			AssertEquals(2, a.Count());\n\
		");
	}

	static void TestIndexerPushBack()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[] = 11;\n\
			a[] = 12;\n\
			a[] = 13;\n\
			\n\
			AssertEquals(11, a[0]);\n\
			AssertEquals(12, a[1]);\n\
			AssertEquals(13, a[2]);\n\
		");
	}

	ArrayTests() : TestFixtureBase("ArrayTests")
	{
		ADD_TESTCASE(TestCountEmpty);
		ADD_TESTCASE(TestPushBackIncreasesCount);
		ADD_TESTCASE(TestPushBackFront);
		ADD_TESTCASE(TestPushBackBack);
		ADD_TESTCASE(TestGetByAutoIndex);
		ADD_TESTCASE(TestIndexerPassThrough);
		ADD_TESTCASE(TestSetGet);
		ADD_TESTCASE(TestSetGetString);
		ADD_TESTCASE(TestContainsKeyReturnsTrue);
		ADD_TESTCASE(TestContainsKeyReturnsFalse);
		ADD_TESTCASE(TestContainsValueReturnsTrue);
		ADD_TESTCASE(TestContainsValueReturnsFalse);
		ADD_TESTCASE(TestRemoveKeyRemoves);
		ADD_TESTCASE(TestRemoveKeyThrowsIfNotFound);
		ADD_TESTCASE(TestRemoveValueRemoves);
		ADD_TESTCASE(TestRemoveValueRemovesOnlyFirst);
		ADD_TESTCASE(TestRemoveValueDoesNothingIfNotFound);
		ADD_TESTCASE(TestIndexerPushBack);
	}
};