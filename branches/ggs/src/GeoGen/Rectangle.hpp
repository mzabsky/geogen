#pragma once

#include <vector>

#include "Point.hpp"
#include "Size.hpp"

namespace geogen {
	class Rectangle{
	private:
		Point position;
		Size2D size;
		Point endingPoint;
	public:
		Rectangle() : position(Point()), size(Size2D()), endingPoint(Point()) {}
		Rectangle(Point position, Size2D size) : position(position), size(size), endingPoint(position.GetX() + size.GetWidth(), position.GetY() + size.GetHeight()) {}

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

		inline Point GetPosition() const { return this->position; }
		inline Size2D GetSize() const { return this->size; }
		inline Point GetEndingPoint() const { return endingPoint; }

		static Rectangle Combine(Rectangle a, Rectangle b);
		static Rectangle Intersect(Rectangle a, Rectangle b);
		static Rectangle Expand(Rectangle a, Size1D size);

		bool Contains(Rectangle other) const;
		bool Contains(Point point) const;
		Point NextPoint(Point point) const;

	};

	/*class RectangleIterator
	{
	private:
		Point point;
		Rectangle rect;
		bool isEnd;

		void Increase()
		{
			Point next = this->rect.NextPoint(this->point);

			if (next.GetY() > this->rect.GetEndingPoint().GetY())
			{
				this->isEnd = true;
			}
		}
	public:
		typedef RectangleIterator self_type;
		typedef Point value_type;
		typedef Point& reference;
		typedef Point* pointer;
		typedef std::forward_iterator_tag bidirectional_iterator_tag;
		typedef int difference_type;
		RectangleIterator(Rectangle rect, Point point, bool isEnd) : rect(rect), point(point), isEnd(isEnd) { }
		
		self_type operator++() 
		{ 
			self_type i = *this; 
			this->Increase();
			return i; 
		}

		self_type operator++(int junk) { this->Increase(); return *this; }
		reference operator*() { return point; }
		pointer operator->() { return &point; }
		bool operator==(const self_type& rhs) { return this->point == rhs.point; }
		bool operator!=(const self_type& rhs) { return this->point != rhs.point; }		
	};*/
}