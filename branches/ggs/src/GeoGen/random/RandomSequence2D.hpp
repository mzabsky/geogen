#pragma once

#include "RandomSequence.hpp"
#include "../Point.hpp"
#include "Hash.hpp"

namespace geogen
{
	namespace random
	{
		/// Pseudo-random sequence which generates a number for each point in 2D space in each step of the sequence. The numbers are determined by the initial seed. This sequence has to be advanced manually.
		class RandomSequence2D
		{
		private:
			mtrand::MTRand_int32 mtRand;
			int current;
		public:

			/// Constructor.
			/// @param seed The seed.
			RandomSequence2D(RandomSeed seed) : mtRand(seed) { this->current = this->mtRand(); }

			/// Advances the sequence.
			inline void Advance() { this->current = this->mtRand(); }

			/// Returns a number corresponding to @a point, in range <INT_MIN, INT_MAX>. Does not advance the sequence.
			/// @param point The point.
			/// @return The number.
			inline int GetInt(Point point) const { return Hash(this->current, point.GetX(), point.GetY()); }
			
			/// Returns a number corresponding to @a point, in range <@a min, @a max>. Does not advance the sequence.
			/// @param point The point.
			/// @return The number.
			inline int GetInt(Point point, int min, int max) { return min + ((unsigned)this->GetInt(point) % (int)(max - min + 1)); }
			
			/// Returns a number corresponding to point [@a coordinate, 0], in range <@a min, @a max>. Does not advance the sequence.
			/// @param coordinate The coordinate.
			/// @return The number.
			inline int GetInt(Coordinate coordinate, int min, int max) { return this->GetInt(Point(coordinate, 0), min, max); }
			
			/// Returns a number corresponding to @a point, in range <HEIGHT_MIN, HEIGHT_MAX>. Does not advance the sequence.
			/// @param point The point.
			/// @return The number.
			inline Height GetHeight(Point point) { return (Height)this->GetInt(point, HEIGHT_MIN, HEIGHT_MAX); }

			/// Returns a number corresponding to @a point, in range <0, 1>. Does not advance the sequence.
			/// @param point The point.
			/// @return The number.
			inline double GetDouble(Point point) { return (double)((unsigned)this->GetInt(point)) * (1. / 4294967295.); }
		};
	}
}