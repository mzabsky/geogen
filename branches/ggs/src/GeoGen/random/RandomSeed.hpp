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