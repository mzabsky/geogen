#pragma once

#include "Point.hpp"
#include "Size.hpp"

namespace geogen {
	class Interval{
	private:
		Coordinate start;
		Size1D length;
	public:
		Interval() : start(0), length(0) {};
		Interval(Coordinate start, Size1D length) : start(start), length(length) {}

		inline Coordinate GetStart() const { return this->start; }
		inline Size1D GetLength() const { return this->length; }

		static Interval Combine(Interval a, Interval b);
	};
}