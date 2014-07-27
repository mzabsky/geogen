#include "Rectangle.hpp"
#include "Size.hpp"

using namespace geogen;

namespace geogen
{	
	Rectangle Rectangle::Combine(Rectangle a, Rectangle b)
	{
		Coordinate returnRectX = a.GetPosition().GetX();
		Size1D returnRectWidth = a.GetSize().GetWidth();
		Coordinate returnRectY = a.GetPosition().GetY();
		Size1D returnRectHeight = a.GetSize().GetHeight();

		if (b.GetPosition().GetX() < a.GetPosition().GetY())
		{
			returnRectWidth += a.GetPosition().GetX() - b.GetPosition().GetX();
			returnRectX = b.GetPosition().GetX();
		}

		if (b.GetSize().GetWidth() > a.GetSize().GetWidth())
		{
			returnRectWidth = b.GetSize().GetWidth();
		}

		if (b.GetPosition().GetY() < a.GetPosition().GetY())
		{
			returnRectHeight += a.GetPosition().GetY() - b.GetPosition().GetY();
			returnRectY = b.GetPosition().GetY();
		}

		if (b.GetSize().GetHeight() > a.GetSize().GetHeight())
		{
			returnRectHeight = b.GetSize().GetHeight();
		}

		return Rectangle(Point(returnRectX, returnRectY), Size2D(returnRectWidth, returnRectHeight));
	}

	Rectangle Rectangle::Intersect(Rectangle a, Rectangle b)
	{
		Coordinate returnRectX = a.GetPosition().GetX();
		Size1D returnRectWidth = a.GetSize().GetWidth();
		Coordinate returnRectY = a.GetPosition().GetY();
		Size1D returnRectHeight = a.GetSize().GetHeight();

		if (b.GetPosition().GetX() > a.GetPosition().GetY())
		{
			returnRectWidth -= a.GetPosition().GetX() - b.GetPosition().GetX();
			returnRectX = b.GetPosition().GetX();
		}

		if (b.GetSize().GetWidth() < a.GetSize().GetWidth())
		{
			returnRectWidth = b.GetSize().GetWidth();
		}

		if (b.GetPosition().GetY() > a.GetPosition().GetY())
		{
			returnRectHeight -= a.GetPosition().GetY() - b.GetPosition().GetY();
			returnRectY = b.GetPosition().GetY();
		}

		if (b.GetSize().GetHeight() < a.GetSize().GetHeight())
		{
			returnRectHeight = b.GetSize().GetHeight();
		}

		return Rectangle(Point(returnRectX, returnRectY), Size2D(returnRectWidth, returnRectHeight));
	}

	Rectangle Rectangle::Expand(Rectangle a, Size1D size)
	{
		// TODO: size check
		return Rectangle(a.GetPosition() - Point(size, size), a.GetSize() + Size2D(2 * size, 2 * size));
	}

	bool Rectangle::Contains(Rectangle other) const
	{
		return
			this->position.GetX() <= other.position.GetX() &&
			this->position.GetY() <= other.position.GetY() &&
			this->position.GetX() + (Coordinate)this->size.GetWidth() >= other.position.GetX() + (Coordinate)other.size.GetWidth() &&
			this->position.GetY() + (Coordinate)this->size.GetHeight() >= other.position.GetY() + (Coordinate)other.size.GetHeight();
	}

	bool Rectangle::Contains(Point point) const
	{
		return
			this->position.GetX() <= point.GetX() &&
			this->position.GetY() <= point.GetY() &&
			this->position.GetX() + (Coordinate)this->size.GetWidth() >= point.GetX() &&
			this->position.GetY() + (Coordinate)this->size.GetHeight() >= point.GetY();
	}

	Point Rectangle::NextPoint(Point point) const
	{
		if (!this->Contains(point))
		{
			return this->position;
		}

		Coordinate x = point.GetX() + 1;
		Coordinate y = point.GetY();

		if (x > this->position.GetX() + (Coordinate)this->size.GetWidth())
		{
			y++;
			x = this->position.GetX();
		}

		return Point(x, y);
	}
}


