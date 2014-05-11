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