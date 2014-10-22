#pragma once

#include <vector>
#include <algorithm>

#include "Point.hpp"
#include "Size.hpp"
#include "Serializable.hpp"
#include "Direction.hpp"

namespace geogen {
	/// A rectangle in 2D space with a position and a size.
	class Rectangle : public Serializable 
	{
	private:
		Point position;
		Size2D size;
		Point endingPoint;
	public:
		/// Default constructor.
		Rectangle() : position(Point()), size(Size2D()), endingPoint(Point()) {}

		/// Constructor.
		/// @param position The position.
		/// @param size The size.
		Rectangle(Point position, Size2D size) :
			position(position), size(size), endingPoint(Coordinate((std::min)((long long)COORDINATE_MAX, (long long)position.GetX() + (long long)size.GetWidth())), Coordinate((std::min)((long long)COORDINATE_MAX, (long long)position.GetY() + (long long)size.GetHeight()))) {}

		/// Constructor. Minimum rectangle containing both the points.
		/// @param firstPoint One point.
		/// @param secondPoint Second point.
		Rectangle(Point firstPoint, Point secondPoint);

		/// Moves the entire rectangle by an offset.
		/// @param other The offset.
		/// @return The moved rectangle.
		inline Rectangle Rectangle::operator+(const Point& other) const
		{
			return Rectangle(Point(this->position.GetX() + other.GetX(), this->position.GetY() + other.GetY()), this->size);
		}

		/// Moves the entire rectangle by an offset.
		/// @param other The offset.
		/// @return The moved rectangle.
		Rectangle& Rectangle::operator+=(const Point& other)
		{
			*this = *this + other;
			return *this;
		}

		/// Increases size of the rectangle by given size.
		/// @param other The size.
		/// @return The resized rectangle.
		inline Rectangle Rectangle::operator+(const Size2D& other) const
		{
			return Rectangle(this->position, Size2D(this->size.GetWidth() + other.GetWidth(), this->size.GetHeight() + other.GetHeight()));
		}

		/// Increases size of the rectangle by given size.
		/// @param other The size.
		/// @return The resized rectangle.
		Rectangle& Rectangle::operator+=(const Size2D& other)
		{
			*this = *this + other;
			return *this;
		}

		/// Moves the entire rectangle by an offset.
		/// @param other The offset.
		/// @return The moved rectangle.
		inline Rectangle Rectangle::operator-(const Point& other) const
		{
			return Rectangle(Point(this->position.GetX() - other.GetX(), this->position.GetY() - other.GetY()), this->size);
		}

		/// Moves the entire rectangle by an offset.
		/// @param other The offset.
		/// @return The moved rectangle.
		Rectangle& Rectangle::operator-=(const Point& other)
		{
			*this = *this - other;
			return *this;
		}

		/// Decreases size of the rectangle by given size.
		/// @param other The size.
		/// @return The resized rectangle.
		inline Rectangle Rectangle::operator-(const Size2D& other) const
		{
			return Rectangle(this->position, Size2D(this->size.GetWidth() - other.GetWidth(), this->size.GetHeight() - other.GetHeight()));
		}

		/// Decreases size of the rectangle by given size.
		/// @param other The size.
		/// @return The resized rectangle.
		Rectangle& Rectangle::operator-=(const Size2D& other)
		{
			*this = *this - other;
			return *this;
		}

		/// Multiplies both size and position of the rectangle by a scale.
		/// @param other The scale.
		/// @return The resized rectangle.
		inline Rectangle Rectangle::operator*(const double other) const
		{
			return Rectangle(this->position * other, this->size * other);
		}

		/// Multiplies both size and position of the rectangle by a scale.
		/// @param other The scale.
		/// @return The resized rectangle.
		Rectangle& Rectangle::operator*=(const double other)
		{
			*this = *this * other;
			return *this;
		}

		/// Gets position of top left corner.
		/// @return The position.
		inline Point GetPosition() const { return this->position; }

		/// Gets the size.
		/// @return The size.
		inline Size2D GetSize() const { return this->size; }

		/// Gets position of bottom right corner.
		/// @return The ending point.
		inline Point GetEndingPoint() const { return endingPoint; }

		/// Combines two rectangles. The result is a bounding rectangle of the two rectangles.
		/// @param a The first rectangle.
		/// @param b The second rectangle.
		/// @return The combined rectangle.
		static Rectangle Combine(Rectangle a, Rectangle b);

		/// Creates an intersection of two rectangles.
		/// @param a The first rectangle.
		/// @param b The second rectangle.
		/// @return The intersection.
		static Rectangle Intersect(Rectangle a, Rectangle b);

		/// Expands a rectangle by given size in all directions.
		/// @param a The rectangle to expand.
		/// @param size The expansion size.
		/// @return The expanded rectangle.
		static Rectangle Expand(Rectangle a, Size1D size);

		/// Expands a rectangle by given size in specified direction.
		/// @param a The rectangle to expand.
		/// @param size The expansion size.
		/// @param direction The expansion direction.
		/// @return The expanded rectangle.
		static Rectangle Expand(Rectangle a, Size1D size, Direction direction);
		
		/// Contracts a rectangle by given size in all directions.
		/// @param a The rectangle to contract.
		/// @param size The contraction size.
		/// @return The contracted rectangle.
		static Rectangle Contract(Rectangle a, Size1D size);

		/// Contracts a rectangle by given size in specified direction.
		/// @param a The rectangle to contract.
		/// @param size The contraction size.
		/// @param direction The contraction direction.
		/// @return The contracted rectangle.
		static Rectangle Contract(Rectangle a, Size1D size, Direction direction);

		/// Checks whether this rectangle fully contains another rectangle.
		/// @param other The contained rectangle.
		/// @return true if this rectangle fully contains the other rectangle, false otherwise.
		bool Contains(Rectangle other) const;

		/// Checks whether this rectangle contains the specified point.
		/// @param point The contained point.
		/// @return true if this rectangle contains the point, false otherwise.
		bool Contains(Point point) const;

		virtual void Serialize(IOStream& stream) const
		{
			stream << GG_STR("[");
			this->position.Serialize(stream);
			stream << GG_STR(", ");
			this->size.Serialize(stream);
			stream << GG_STR("]");
		}
	};

	/// A rectangle covering the entire representable 2D space.
	const Rectangle RECTANGLE_MAX = Rectangle(Point(COORDINATE_MIN, COORDINATE_MIN), SIZE2D_MAX);
}