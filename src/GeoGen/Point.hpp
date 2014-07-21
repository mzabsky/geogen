#pragma once

#include <climits>

namespace geogen {
	typedef int Coordinate;

	const Coordinate COORDINATE_MAX = INT_MAX;
	const Coordinate COORDINATE_MIN = INT_MIN;

	class Point{
	private:
		Coordinate x, y;
	public:
		Point() : x(0), y(0) {}
		Point(Coordinate x, Coordinate y) : x(x), y(y) {}

		inline Coordinate GetX() const { return this->x; }
		inline Coordinate GetY() const { return this->y; }
	};
}