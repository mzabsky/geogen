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

	static void TestRandomSequence2D()
	{
		RandomSequence2D sequence(8);

		ASSERT_EQUALS(int, -1264649449, sequence.GetInt(Point(0, 0)));
		ASSERT_EQUALS(int, -1264649449, sequence.GetInt(Point(0, 0)));
		ASSERT_EQUALS(int, -1104168230, sequence.GetInt(Point(0, 1)));
		ASSERT_EQUALS(int, 945191568, sequence.GetInt(Point(1, 0)));
		ASSERT_EQUALS(int, -943687011, sequence.GetInt(Point(0, 2)));
	}

	static void TestRandomSequenceHistogram()
	{
		/*RandomSequence sequence(8);

		const unsigned count = 1000;
		unsigned* histogram = new unsigned[count];

		for (unsigned i = 0; i < count; i++)
		{
			histogram[i] = 0;
		}

		for (unsigned i = 0; i < 1000000; i++)
		{
			histogram[sequence.NextInt(0, count - 1)]++;
		}

		for (unsigned i = 0; i < count; i++)
		{
			cout << i << "\t" << histogram[i] << endl;
		}*/
	}

	RandomTests() : TestFixtureBase("RandomTests")
	{
		ADD_TESTCASE(TestSimpleSequence);
		ADD_TESTCASE(TestParalellSequences);
		ADD_TESTCASE(TestRandomSequence2D);
		ADD_TESTCASE(TestRandomSequenceHistogram);
	}
};