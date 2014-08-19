#pragma once

#include "../String.hpp"

namespace geogen
{
	namespace random
	{
		typedef unsigned RandomSeed;

		RandomSeed CreateSeed(unsigned n);
		RandomSeed CreateSeed(int n);
		RandomSeed CreateSeed(unsigned long long n);
		RandomSeed CreateSeed(long long n);
		RandomSeed CreateSeed(double n);
		RandomSeed CreateSeed(String str);
		RandomSeed RecognizeSeed(String str);

		RandomSeed CombineSeeds(RandomSeed s1, RandomSeed s2);
		RandomSeed CombineSeeds(RandomSeed s1, RandomSeed s2, RandomSeed s3);
		RandomSeed CombineSeeds(RandomSeed s1, RandomSeed s2, RandomSeed s3, RandomSeed s4);
		RandomSeed CombineSeeds(RandomSeed s1, RandomSeed s2, RandomSeed s3, RandomSeed s4, RandomSeed s5);
	}
}