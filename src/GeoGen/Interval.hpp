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

#include "Point.hpp"
#include "Size.hpp"
#include "Rectangle.hpp"
#include "Direction.hpp"

namespace geogen {
	/// A coordinate interval in a 1D space.
	class Interval : public Serializable
	{
	private:
		Coordinate start;
		Size1D length;
		Coordinate end;
	public:
		/// Default constructor.
		Interval() : start(0), length(0), end(0) {};

		/// Constructor.
		/// @param start The starting coordinate.
		/// @param length The length.
		Interval(Coordinate start, Size1D length) : start(start), length(length), end(start + length) {}

		/// Creates a minimum interval containing both coordinates.
		/// @param first The first coordinate.
		/// @param second The second coordinate.
		/// @return An interval.
		static inline Interval FromCoordinates(Coordinate start, Coordinate end) 
		{
			return Combine(Interval(start, 1), Interval(end, 1));
		}

		/// Gets the starting coordinate.
		/// @return The starting coordinate.
		inline Coordinate GetStart() const { return this->start; }

		/// Gets the length.
		/// @return The length.
		inline Size1D GetLength() const { return this->length; }

		/// Gets the ending coordinate.
		/// @return The ending coordinate.
		inline Coordinate GetEnd() const { return this->end; }

		/// Moves the entire interval by an offset.
		/// @param other The offset.
		/// @return The moved interval.
		inline Interval operator+(const Coordinate other) const
		{
			return Interval(this->start + other, this->length);
		}

		/// Moves the entire interval by an offset.
		/// @param other The offset.
		/// @return The moved interval.
		Interval& operator+=(const Coordinate other)
		{
			*this = *this + other;
			return *this;
		}

		/// Increases length of the interval by given size.
		/// @param other The size.
		/// @return The new interval.
		inline Interval operator+(const Size1D other) const
		{
			return Interval(this->start, this->length + other);
		}

		/// Increases length of the interval by given size.
		/// @param other The size.
		/// @return The new interval.
		Interval& operator+=(const Size1D other)
		{
			*this = *this + other;
			return *this;
		}

		/// Moves the entire interval by an offset.
		/// @param other The offset.
		/// @return The moved interval.
		inline Interval operator-(const Coordinate other) const
		{
			return Interval(this->start - other, this->length);
		}

		/// Moves the entire interval by an offset.
		/// @param other The offset.
		/// @return The moved interval.
		Interval& operator-=(const Coordinate other)
		{
			*this = *this - other;
			return *this;
		}

		/// Decreases length of the interval by given size.
		/// @param other The size.
		/// @return The new interval.
		inline Interval operator-(const Size1D& other) const
		{
			return Interval(this->start, this->length - other);
		}

		/// Decreases length of the interval by given size.
		/// @param other The size.
		/// @return The new interval.
		Interval& operator-=(const Size1D& other)
		{
			*this = *this - other;
			return *this;
		}

		/// Multiplies both starting coordinate and length of the rectangle by a scale.
		/// @param other The scale.
		/// @return The resized interval.
		inline Interval operator*(const double other) const
		{
			return Interval(Coordinate(this->start * other), Size1D(this->length * other));
		}

		/// Multiplies both starting coordinate and length of the rectangle by a scale.
		/// @param other The scale.
		/// @return The resized interval.
		Interval& operator*=(const double other)
		{
			*this = *this * other;
			return *this;
		}

		/// Divides both starting coordinate and length of the rectangle by a scale.
		/// @param other The scale.
		/// @return The resized interval.
		inline Interval operator/(const double other) const
		{
			return Interval(Coordinate(this->start / other), Size1D(this->length / other));
		}

		/// Divides both starting coordinate and length of the rectangle by a scale.
		/// @param other The scale.
		/// @return The resized interval.
		Interval& operator/=(const double other)
		{
			*this = *this * other;
			return *this;
		}

		bool Contains(Interval other) const;
		bool Contains(Coordinate x) const;

		static Interval Combine(Interval a, Interval b);
		static Interval Intersect(Interval a, Interval b);
		static Interval FromRectangle(Rectangle rect, Direction direction);
		static Interval Expand(Interval interval, Size1D distance);

		void Serialize(IOStream& stream) const
		{
			stream << GG_STR("[") << this->start << GG_STR(", ") << this->length << GG_STR("]");
		}
	};

	/// The maximum representable interval covering the entire 1D space.
	const Interval INTERVAL_MAX = Interval(COORDINATE_MIN, SIZE1D_MAX);
}