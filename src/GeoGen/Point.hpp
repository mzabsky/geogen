#pragma once

#include <climits>

#include "Size.hpp"

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

		inline Point Point::operator+(const Point& other) const 
		{
			return Point(this->x + other.x, this->y + other.y);
		}

		Point& Point::operator+=(const Point& other)
		{
			*this = *this + other;
			return *this;
		}

		inline Point Point::operator+(const Size2D& other) const
		{
			return Point(this->x + other.GetWidth(), this->y + other.GetHeight());
		}

		Point& Point::operator+=(const Size2D& other)
		{
			*this = *this + other;
			return *this;
		}

		inline Point Point::operator-(const Point& other) const
		{
			return Point(this->x - other.x, this->y - other.y);
		}

		Point& Point::operator-=(const Point& other)
		{
			*this = *this - other;
			return *this;
		}

		inline Point Point::operator-(const Size2D& other) const
		{
			return Point(this->x - other.GetWidth(), this->y - other.GetHeight());
		}

		Point& Point::operator-=(const Size2D& other)
		{
			*this = *this - other;
			return *this;
		}

		inline Coordinate GetX() const { return this->x; }
		inline Coordinate GetY() const { return this->y; }
	};
}