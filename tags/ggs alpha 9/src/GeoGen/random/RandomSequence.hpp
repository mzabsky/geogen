#pragma once

#include "RandomSeed.hpp"
#include "../Number.hpp"
#include "mtrand/MTRand.hpp"

namespace geogen
{
	namespace random
	{
		/// Pseudo-random sequence of number determined by its seed.
		class RandomSequence
		{
		private:
			mtrand::MTRand_int32 mtRand;
		public:

			/// Constructor.
			/// @param seed The seed.
			RandomSequence(RandomSeed seed);

			/// Returns the next number in the sequence in range <INT_MIN, INT_MAX> and advances the sequence.
			/// @return The number.
			inline int NextInt() { return this->mtRand(); }
			
			/// Returns the next number in the sequence in range <0, UINT_MAX> and advances the sequence.
			/// @return The number.
			inline int NextUInt() { return (unsigned)this->mtRand(); }
			
			/// Returns the next number in the sequence in range <@a min, @a max> and advances the sequence.
			/// @return The number.
			inline int NextInt(int min, int max) { return min + ((unsigned)this->NextInt() % (int)(max - min + 1)); }
			
			/// Returns the next number in the sequence in range <@a min, @a max> and advances the sequence.
			/// @return The number.
			inline int NextUInt(unsigned min, unsigned max) { return min + ((unsigned)this->NextInt() % (unsigned)(max - min + 1)); }
			
			/// Returns the next number in the sequence in range <geogen::HEIGHT_MIN, geogen::HEIGHT_MAX> and advances the sequence.
			/// @return The number.
			inline Height NextHeight() { return (Height)this->NextInt(HEIGHT_MIN, HEIGHT_MAX); }

			/// Returns the next number in the sequence in range <0, 1> and advances the sequence.
			/// @return The number.
			inline double NextDouble() { return (double)((unsigned)this->NextInt()) * (1. / 4294967295.); }
		};
	}
}