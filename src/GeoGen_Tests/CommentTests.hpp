#pragma once

#include "TestFixtureBase.hpp"

class CommentTests : public TestFixtureBase
{
public:
	static void TestLineComment()
	{
		TestScript("\n\
			var a = 1;\n\
			// a = 2; \n\
			\n\
			AssertEquals(1, a);\n\
		");
	}

	static void TestBlockComment()
	{
		TestScript("\n\
			var a = 1;\n\
			/* \n\
			a = 2; \n\
			*/ \n\
			\n\
			AssertEquals(1, a);\n\
		");
	}

		static void TestCombinedComment()
	{
		TestScript("\n\
			var a = 1;\n\
			/* \n\
			// */ \n\
			a = 2; \n\
			/* */ \n\
			\n\
			AssertEquals(1, a);\n\
		");
	}

	CommentTests() : TestFixtureBase("CommentTests")
	{
		ADD_TESTCASE(TestLineComment);
		ADD_TESTCASE(TestBlockComment);
	}
};