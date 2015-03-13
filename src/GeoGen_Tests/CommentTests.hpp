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
			AssertEquals(2, a);\n\
		");
	}

	CommentTests() : TestFixtureBase("CommentTests")
	{
		ADD_TESTCASE(TestLineComment);
		ADD_TESTCASE(TestBlockComment);
		ADD_TESTCASE(TestCombinedComment);
	}
};