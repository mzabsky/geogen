#pragma once

#include "Point.hpp"
#include "Size.hpp"

namespace geogen {
	class Rectangle{
	private:
		Point position;
		Size2D size;
	public:
		Rectangle() : position(Point()), size(Size2D()) {}
		Rectangle(Point position, Size2D size) : position(position), size(size) {}

		inline Point GetPosition() const { return this->position; }
		inline Size2D GetSize() const { return this->size; }

		static Rectangle Combine(Rectangle a, Rectangle b);
	};
}