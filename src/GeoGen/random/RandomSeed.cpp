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

#include "RandomSeed.hpp"
#include "Hash.hpp"

using namespace std;
using namespace geogen;
using namespace random;

RandomSeed geogen::random::CreateSeed(unsigned n)
{
	return Hash(n);
}

RandomSeed geogen::random::CreateSeed(int n)
{
	return Hash((unsigned)n);
}

RandomSeed geogen::random::CreateSeed(unsigned long long n)
{
	return Hash((unsigned)n, (unsigned)n >> sizeof(unsigned));
}

RandomSeed geogen::random::CreateSeed(long long n)
{
	return CreateSeed((unsigned long long)n);
}

RandomSeed geogen::random::CreateSeed(double n)
{
	// There needs to be tolerance for divergences in very small numbers
	return CreateSeed((long long)(n * 10000));
}

RandomSeed geogen::random::CreateSeed(String str)
{
	return Hash(str);
}

RandomSeed geogen::random::RecognizeSeed(String str)
{
	RandomSeed numericSeed;
	StringStream ss(str);
	ss >> numericSeed;

	if (!ss.fail())
	{
		return numericSeed;
	}
	else
	{
		return CreateSeed(str);
	}	
}

RandomSeed geogen::random::CombineSeeds(RandomSeed s1, RandomSeed s2)
{
	return Hash(s1, s2);
}

RandomSeed geogen::random::CombineSeeds(RandomSeed s1, RandomSeed s2, RandomSeed s3)
{
	return Hash(s1, s2, s3);
}

RandomSeed geogen::random::CombineSeeds(RandomSeed s1, RandomSeed s2, RandomSeed s3, RandomSeed s4)
{
	return Hash(s1, s2, s3, s4);
}

RandomSeed geogen::random::CombineSeeds(RandomSeed s1, RandomSeed s2, RandomSeed s3, RandomSeed s4, RandomSeed s5)
{
	return Hash(s1, s2, s3, s4, s5);
}