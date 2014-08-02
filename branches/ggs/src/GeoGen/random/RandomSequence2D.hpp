#pragma once

#include "RandomSequence.hpp"
#include "../Point.hpp"
#include "Hash.hpp"

namespace geogen
{
	namespace random
	{
		class RandomSequence2D
		{
		private:
			mtrand::MTRand_int32 mtRand;
			int current;
		public:
			RandomSequence2D(RandomSeed seed) : mtRand(seed) { this->current = this->mtRand(); }

			inline void Advance() { this->current = this->mtRand(); }

			inline int GetInt(Point point) const { return Hash(this->current, point.GetX(), point.GetY()); }
			inline int GetInt(Point point, int min, int max) { return min + (this->GetInt(point) % (int)(max - min + 1)); }
			inline Height GetHeight(Point point) { return (Height)this->GetInt(point, HEIGHT_MIN, HEIGHT_MAX); }
			inline double GetDouble(Point point) { return (double)(this->GetInt(point)) * (1. / 4294967295.); }
		};
	}
}