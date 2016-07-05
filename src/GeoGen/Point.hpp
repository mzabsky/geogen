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

#include <climits>
#include <algorithm>
#include <math.h>

#include "Size.hpp"
#include "Serializable.hpp"

namespace geogen {
	/// A coordinate in a 1D space. May be positive and negative.
	typedef int Coordinate;

	/// The maximum coordinate.
	const Coordinate COORDINATE_MAX = INT_MAX;

	/// The minimum coordinate.
	const Coordinate COORDINATE_MIN = INT_MIN;

	/// A point in 2D space.
	class Point : public Serializable
	{
	private:
		Coordinate x, y;
	public:
		/// Default constructor.
		Point() : x(0), y(0) {}

		/// Constructor.
		/// @param x The Coordinate to process.
		/// @param y The Coordinate to process.
		Point(Coordinate x, Coordinate y) : x(x), y(y) {}

		/// Adds both components of the two points together separately.
		/// @param other The other point.
		/// @return The result.
		inline Point operator+(const Point& other) const 
		{
			return Point(this->x + other.x, this->y + other.y);
		}

		/// Adds both components of the two points together separately.
		/// @param other The other point.
		/// @return The result.
		Point& operator+=(const Point& other)
		{
			*this = *this + other;
			return *this;
		}

		inline Point operator+(const Size2D& other) const
		{
			return Point(this->x + other.GetWidth(), this->y + other.GetHeight());
		}

		Point& operator+=(const Size2D& other)
		{
			*this = *this + other;
			return *this;
		}

		inline Point operator-(const Point& other) const
		{
			return Point(this->x - other.x, this->y - other.y);
		}

		Point& operator-=(const Point& other)
		{
			*this = *this - other;
			return *this;
		}

		inline Point operator-(const Size2D& other) const
		{
			return Point(this->x - other.GetWidth(), this->y - other.GetHeight());
		}

		Point& operator-=(const Size2D& other)
		{
			*this = *this - other;
			return *this;
		}

		inline Point operator*(const double other) const
		{
			return Point((Size1D)(this->x * other), (Size1D)(this->y * other));
		}

		Point& operator*=(const double other)
		{
			*this = *this * other;
			return *this;
		}

		inline Point operator/(const double other) const
		{
			return Point((Size1D)(this->x / other), (Size1D)(this->y / other));
		}

		Point& operator/=(const double other)
		{
			*this = *this / other;
			return *this;
		}

		/// Gets the X coordinate.
		/// @return The X coordinate.
		inline Coordinate GetX() const { return this->x; }

		/// Gets the Y coordinate.
		/// @return The Y coordinate.
		inline Coordinate GetY() const { return this->y; }

		/// Calculates euclidean distance between this point and another point.
		/// @param destination The other point.
		/// @return The distance.
		inline double GetDistanceTo(Point destination) const
		{
			return sqrt((double)(((long long)destination.x - (long long)this->x) * ((long long)destination.x - (long long)this->x) + ((long long)destination.y - (long long)this->y) * ((long long)destination.y - (long long)this->y)));
		}

		/// Gets euclidean distance between this point and the origin (point [0,0]).
		/// @return The distance.
		inline double GetDistanceFromOrigin() const
		{
			return sqrt((double)((long long)this->x * (long long)this->x + (long long)this->y * (long long)this->y));
		}

		/// Gets top left point of a grid cell to which this point belongs to.
		/// @param gridCellWidth Grid cell width.
		/// @param gridCellHeight Grid cell height.
		/// @return The distance.
		inline Point GetGridPoint(unsigned gridCellWidth, unsigned gridCellHeight)
		{
			return Point(this->x - this->x % gridCellWidth, this->y - this->y % gridCellHeight);
		}

		void Serialize(IOStream& stream) const
		{
			stream << GG_STR("[") << this->x << GG_STR(", ") << this->y << GG_STR("]");
		}
	};

	/// Next coordinate that is a multiple of given number. If the coordinate is already a multiple of that number, the same coordinate will be returned.
	/// @param coord The coordinate.
	/// @param multiplier The multiplier.
	/// @return A next multiple.
	inline Coordinate NextMultipleOfInclusive(Coordinate coord, unsigned multiplier)
	{
		if (coord < 0)
		{
			int remainder = abs(coord) % multiplier;
			return -Coordinate(abs(coord) - remainder);
		}
		else
		{
			return coord + multiplier - 1 - (coord - 1) % multiplier;
		}
	}

	/// Next coordinate that is a multiple of given number. If the coordinate is already a multiple of that number, the next multiple will be returned.
	/// @param coord The coordinate.
	/// @param multiplier The multiplier.
	/// @return A next multiple.
	inline Coordinate NextMultipleOfExclusive(Coordinate coord, unsigned multiplier)
	{
		if (coord < 0)
		{
			int remainder = abs(coord) % multiplier;
			if (remainder == 0)
				return -Coordinate(abs(coord) - multiplier);
			return -(abs(coord) - remainder);			
		}
		else
		{
			return coord + multiplier - coord % multiplier;
		}
	}

	/// Previous coordinate that is a multiple of given number. If the coordinate is already a multiple of that number, the same coordinate will be returned.
	/// @param coord The coordinate.
	/// @param multiplier The multiplier.
	/// @return A previous multiple.
	inline Coordinate PreviousMultipleOfInclusive(Coordinate coord, unsigned multiplier)
	{
		if (coord < 0)
		{
			return -Coordinate(abs(coord) + multiplier - 1 - (abs(coord) - 1) % multiplier);
		}
		else
		{
			int remainder = coord % multiplier;
			return coord - remainder;
		}
	}

	/// Previous coordinate that is a multiple of given number. If the coordinate is already a multiple of that number, the previous multiple will be returned.
	/// @param coord The coordinate.
	/// @param multiplier The multiplier.
	/// @return A previous multiple.
	inline Coordinate PreviousMultipleOfExclusive(Coordinate coord, unsigned multiplier)
	{
		if (coord < 0)
		{
			return -Coordinate(abs(coord) + multiplier - abs(coord) % multiplier);
		}
		else
		{
			int remainder = coord % multiplier;
			if (remainder == 0)
				return coord - multiplier;
			return coord - remainder;
		}
	}

	/// Attempts to convert a point to a 2D size.
	/// @param p The point to convert.
	/// @param out The conversion result.
	/// @return true if the conversion was successful, false otherwise.
	static inline bool TryPointToSize(Point p, Size2D& out)
	{
		if (p.GetX() < 0 || p.GetY() < 0)
		{
			return false;
		}
		{
			out = Size2D(Size1D(p.GetX()), Size1D(p.GetY()));
			return true;
		}
	}
}