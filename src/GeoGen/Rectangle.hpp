#pragma once

#include <vector>

#include "Point.hpp"
#include "Size.hpp"
#include "Serializable.hpp"
#include "Direction.hpp"

namespace geogen {
	class Rectangle : public Serializable 
	{
	private:
		Point position;
		Size2D size;
		Point endingPoint;
	public:
		Rectangle() : position(Point()), size(Size2D()), endingPoint(Point()) {}
		Rectangle(Point position, Size2D size) : 
			position(position), size(size), endingPoint(Coordinate((long long)position.GetX() + (long long)size.GetWidth()), Coordinate((long long)position.GetY() + (long long)size.GetHeight())) {}

		inline Rectangle Rectangle::operator+(const Point& other) const
		{
			return Rectangle(Point(this->position.GetX() + other.GetX(), this->position.GetY() + other.GetY()), this->size);
		}

		Rectangle& Rectangle::operator+=(const Point& other)
		{
			*this = *this + other;
			return *this;
		}

		inline Rectangle Rectangle::operator+(const Size2D& other) const
		{
			return Rectangle(this->position, Size2D(this->size.GetWidth() + other.GetWidth(), this->size.GetHeight() + other.GetHeight()));
		}

		Rectangle& Rectangle::operator+=(const Size2D& other)
		{
			*this = *this + other;
			return *this;
		}

		inline Rectangle Rectangle::operator-(const Point& other) const
		{
			return Rectangle(Point(this->position.GetX() - other.GetX(), this->position.GetY() - other.GetY()), this->size);
		}

		Rectangle& Rectangle::operator-=(const Point& other)
		{
			*this = *this - other;
			return *this;
		}

		inline Rectangle Rectangle::operator-(const Size2D& other) const
		{
			return Rectangle(this->position, Size2D(this->size.GetWidth() - other.GetWidth(), this->size.GetHeight() - other.GetHeight()));
		}

		Rectangle& Rectangle::operator-=(const Size2D& other)
		{
			*this = *this - other;
			return *this;
		}

		inline Rectangle Rectangle::operator*(const double other) const
		{
			return Rectangle(this->position * other, this->size * other);
		}

		Rectangle& Rectangle::operator*=(const double other)
		{
			*this = *this * other;
			return *this;
		}

		inline Point GetPosition() const { return this->position; }
		inline Size2D GetSize() const { return this->size; }
		inline Point GetEndingPoint() const { return endingPoint; }

		static Rectangle Combine(Rectangle a, Rectangle b);
		static Rectangle Intersect(Rectangle a, Rectangle b);
		static Rectangle Expand(Rectangle a, Size1D size);
		static Rectangle Expand(Rectangle a, Size1D size, Direction direction);
		static Rectangle Contract(Rectangle a, Size1D size);
		static Rectangle Contract(Rectangle a, Size1D size, Direction direction);

		bool Contains(Rectangle other) const;
		bool Contains(Point point) const;
		Point NextPoint(Point point) const;

		virtual void Serialize(IOStream& stream) const
		{
			stream << GG_STR("[");
			this->position.Serialize(stream);
			stream << GG_STR(", ");
			this->size.Serialize(stream);
			stream << GG_STR("]");
		}
	};

	const Rectangle RECTANGLE_MAX = Rectangle(Point(COORDINATE_MIN, COORDINATE_MIN), SIZE2D_MAX);
}