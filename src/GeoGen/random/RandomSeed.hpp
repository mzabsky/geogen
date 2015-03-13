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

#include "../String.hpp"

namespace geogen
{
	namespace random
	{
		/// Numeric pseudo-random generator seed.
		typedef unsigned RandomSeed;

		/// Creates a random seed from a number.
		/// @param n The number.
		/// @return The seed.
		RandomSeed CreateSeed(unsigned n);
		
		/// Creates a random seed from a number.
		/// @param n The number.
		/// @return The seed.
		RandomSeed CreateSeed(int n);
		
		/// Creates a random seed from a number.
		/// @param n The number.
		/// @return The seed.
		RandomSeed CreateSeed(unsigned long long n);
		
		/// Creates a random seed from a number.
		/// @param n The number.
		/// @return The seed.
		RandomSeed CreateSeed(long long n);
		
		/// Creates a random seed from a number.
		/// @param n The number.
		/// @return The seed.
		RandomSeed CreateSeed(double n);

		/// Creates a random seed from a string, using a hash function.
		/// @param str The string.
		/// @return The seed.
		RandomSeed CreateSeed(String str);

		/// Creates a random seed from a string, attempting to read it as a number first and if it fails, it uses a hash function.
		/// @param str The string.
		/// @return The seed.
		RandomSeed RecognizeSeed(String str);

		/// Combine two random seeds into one, deterministically.
		/// @param s1 The first seed.
		/// @param s2 The second seed.
		/// @return The combined.
		RandomSeed CombineSeeds(RandomSeed s1, RandomSeed s2);
		
		/// Combine three random seeds into one, deterministically.
		/// @param s1 The first seed.
		/// @param s2 The second seed.
		/// @param s3 The third seed.
		/// @return The combined.
		RandomSeed CombineSeeds(RandomSeed s1, RandomSeed s2, RandomSeed s3);
		
		/// Combine four random seeds into one, deterministically.
		/// @param s1 The first seed.
		/// @param s2 The second seed.
		/// @param s3 The third seed.
		/// @param s4 The fourth seed.
		/// @return The combined.
		RandomSeed CombineSeeds(RandomSeed s1, RandomSeed s2, RandomSeed s3, RandomSeed s4);

		/// Combine five random seeds into one, deterministically.
		/// @param s1 The first seed.
		/// @param s2 The second seed.
		/// @param s3 The third seed.
		/// @param s4 The fourth seed.
		/// @param s5 The fourth seed.
		/// @return The combined.
		RandomSeed CombineSeeds(RandomSeed s1, RandomSeed s2, RandomSeed s3, RandomSeed s4, RandomSeed s5);
	}
}