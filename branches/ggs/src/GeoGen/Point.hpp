#pragma once

#include <climits>
#include <algorithm>
#include <math.h>

#include "Size.hpp"
#include "Serializable.hpp"

namespace geogen {
	typedef int Coordinate;

	const Coordinate COORDINATE_MAX = INT_MAX;
	const Coordinate COORDINATE_MIN = INT_MIN;

	class Point : public Serializable
	{
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

		inline Point Point::operator*(const double other) const
		{
			return Point((Size1D)(this->x * other), (Size1D)(this->y * other));
		}

		Point& Point::operator*=(const double other)
		{
			*this = *this * other;
			return *this;
		}

		inline Coordinate GetX() const { return this->x; }
		inline Coordinate GetY() const { return this->y; }

		inline unsigned long long GetDistanceTo(Point destination) const
		{
			return (unsigned long)sqrt((double)(((long long)destination.x - (long long)this->x) * ((long long)destination.x - (long long)this->x) + ((long long)destination.y - (long long)this->y) * ((long long)destination.y - (long long)this->y)));
		}

		void Serialize(IOStream& stream) const
		{
			stream << GG_STR("[") << this->x << GG_STR(", ") << this->y << GG_STR("]");
		}
	};
}