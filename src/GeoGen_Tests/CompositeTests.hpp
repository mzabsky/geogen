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

class CompositeTests : public TestFixtureBase
{
public:
	static void TestSieveOfEratosthenes()
	{
		TestScript("\n\
			function Sieve(n){\n\
				var values = Array.Empty();\n\
				for (var i = 1; i <= n; i = i + 1) {\n\
					values[i] = true;\n\
				} \n\
				\n\
				var p = 2;\n\
				while(p * p <= n) {\n\
					var j = p * p;\n\
					while(j <= n){\n\
						values[j] = false;\n\
						j = j + p;\n\
					}\n\
					\n\
					 p = p + 1; \n\
					while(!values[p]) p = p + 1; \n\
				} \n\
				\n\
				var results = Array.Empty();\n\
				for (var i = 1; i <= n; i = i + 1) {\n\
					if(values[i]){\n\
						results.PushBack(i);\n\
					}\n\
				}\n\
				\n\
				return results;\n\
			}\n\
			\n\
			AssertEquals(9, Sieve(20).Count());\n\
		");
	}

	CompositeTests() : TestFixtureBase("CompositeTests")
	{
		ADD_TESTCASE(TestSieveOfEratosthenes);
	}
};