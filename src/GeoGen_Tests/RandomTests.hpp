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

		ASSERT_EQUALS(int, 1860076204, sequence.GetInt(Point(0, 0)));
		ASSERT_EQUALS(int, 1860076204, sequence.GetInt(Point(0, 0)));
		ASSERT_EQUALS(int, 806232052, sequence.GetInt(Point(0, 1)));
		ASSERT_EQUALS(int, -247555535, sequence.GetInt(Point(1, 0)));
		ASSERT_EQUALS(int, -1143852053, sequence.GetInt(Point(0, 2)));
	}
	 
	static void TestRandomSequenceHistogram()
	{
		RandomSequence sequence(8);

		const unsigned range = 5;
		const unsigned count = 100000;
		auto_ptr<unsigned> histogram = auto_ptr<unsigned>(new unsigned[range]);

		for (unsigned i = 0; i < range; i++)
		{
			histogram.get()[i] = 0;
		}

		for (unsigned i = 0; i < count; i++)
		{
			/*int min = 0;
			int max = count - 1;
			unsigned r = (unsigned)sequence.NextInt();
			int act = min + (r % (int)(max - min + 1));

			cout << r << " -> " << act << endl;


			histogram[act]++;*/

			/*int x = sequence.NextInt();
			if (x < count)
			{
				histogram[x]++;
			}*/

			histogram.get()[sequence.NextInt(0, range - 1)]++;
		}

		for (unsigned i = 0; i < range; i++)
		{
			int actual = histogram.get()[i];
			int expected = count / range;
			int deviation = abs(expected - actual);
			int maxDeviation = (int)(expected * 0.01);
			ASSERT_EQUALS(bool, true, deviation < maxDeviation);
		}
	}

	RandomTests() : TestFixtureBase("RandomTests")
	{
		ADD_TESTCASE(TestSimpleSequence);
		ADD_TESTCASE(TestParalellSequences);
		ADD_TESTCASE(TestRandomSequence2D);
		ADD_TESTCASE(TestRandomSequenceHistogram);
	}
};