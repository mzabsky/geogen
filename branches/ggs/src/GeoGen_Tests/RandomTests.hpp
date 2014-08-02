#pragma once

#include "TestFixtureBase.hpp"

class RandomTests : public TestFixtureBase
{
private:
public:
	static void TestSimpleSequence()
	{
		RandomSequence sequence(8);
		ASSERT_EQUALS(int, -543616573, sequence.NextInt());
		ASSERT_EQUALS(int, 47736148, sequence.NextInt());
		ASSERT_EQUALS(int, -135116815, sequence.NextInt());
		ASSERT_EQUALS(int, 1028385129, sequence.NextInt());
		ASSERT_EQUALS(int, -561805179, sequence.NextInt());
	}

	static void TestParalellSequences()
	{
		RandomSequence sequence1(8);
		RandomSequence sequence2(8);
		ASSERT_EQUALS(int, -543616573, sequence1.NextInt());
		sequence2.NextInt();
		ASSERT_EQUALS(int, 47736148, sequence1.NextInt());
		sequence2.NextInt();
		ASSERT_EQUALS(int, -135116815, sequence1.NextInt());
		sequence2.NextInt();
		ASSERT_EQUALS(int, 1028385129, sequence1.NextInt());
		sequence2.NextInt();
		ASSERT_EQUALS(int, -561805179, sequence1.NextInt());
		sequence2.NextInt();
	}

	RandomTests() : TestFixtureBase("RandomTests")
	{
		ADD_TESTCASE(TestSimpleSequence);
		ADD_TESTCASE(TestParalellSequences);
	}
};