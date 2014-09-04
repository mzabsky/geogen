#pragma once

#include "Point.hpp"
#include "Size.hpp"
#include "Rectangle.hpp"
#include "Direction.hpp"

namespace geogen {
	class Interval : public Serializable
	{
	private:
		Coordinate start;
		Size1D length;
		Coordinate end;
	public:
		Interval() : start(0), length(0), end(0) {};
		Interval(Coordinate start, Size1D length) : start(start), length(length), end(start + length) {}

		inline Coordinate GetStart() const { return this->start; }
		inline Size1D GetLength() const { return this->length; }
		inline Coordinate GetEnd() const { return this->end; }

		inline Interval Interval::operator+(const Coordinate other) const
		{
			return Interval(this->start + other, this->length);
		}

		Interval& Interval::operator+=(const Coordinate other)
		{
			*this = *this + other;
			return *this;
		}

		inline Interval Interval::operator+(const Size1D other) const
		{
			return Interval(this->start, this->length + other);
		}

		Interval& Interval::operator+=(const Size1D other)
		{
			*this = *this + other;
			return *this;
		}

		inline Interval Interval::operator-(const Coordinate other) const
		{
			return Interval(this->start - other, this->length);
		}

		Interval& Interval::operator-=(const Coordinate other)
		{
			*this = *this - other;
			return *this;
		}

		inline Interval Interval::operator-(const Size1D& other) const
		{
			return Interval(this->start, this->length - other);
		}

		Interval& Interval::operator-=(const Size1D& other)
		{
			*this = *this - other;
			return *this;
		}

		inline Interval Interval::operator*(const double other) const
		{
			return Interval(Coordinate(this->start * other), Size1D(this->length * other));
		}

		Interval& Interval::operator*=(const double other)
		{
			*this = *this * other;
			return *this;
		}

		inline Interval Interval::operator/(const double other) const
		{
			return Interval(Coordinate(this->start / other), Size1D(this->length / other));
		}

		Interval& Interval::operator/=(const double other)
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

	const Interval INTERVAL_MAX = Interval(COORDINATE_MIN, SIZE1D_MAX);
}