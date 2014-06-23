#pragma once

namespace geogen {
	typedef int Coordinate;

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