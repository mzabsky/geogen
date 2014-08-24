#include "Rectangle.hpp"
#include "Size.hpp"
#include "ApiUsageException.hpp"

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
			returnRectWidth -= b.GetPosition().GetX() - a.GetPosition().GetX();
			returnRectX = b.GetPosition().GetX();
		}

		if (b.GetSize().GetWidth() < a.GetSize().GetWidth())
		{
			returnRectWidth = b.GetSize().GetWidth();
		}

		if (b.GetPosition().GetY() > a.GetPosition().GetY())
		{
			returnRectHeight -= b.GetPosition().GetY() - a.GetPosition().GetY();
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

	Rectangle Rectangle::Expand(Rectangle a, Size1D size, Direction direction)
	{
		// TODO: size check
		if (direction == DIRECTION_HORIZONTAL)
		{
			return Rectangle(a.GetPosition() - Point(size, 0), a.GetSize() + Size2D(2 * size, 0));
		}
		else if (direction == DIRECTION_VERTICAL)
		{
			return Rectangle(a.GetPosition() - Point(0, size), a.GetSize() + Size2D(0, 2 * size));
		}
		else throw ApiUsageException(GG_STR("Invalid direction."));
	}

	Rectangle Rectangle::Contract(Rectangle a, Size1D size)
	{
		// TODO: size check
		return Rectangle(a.GetPosition() + Point(size, size), Size2D(a.GetSize().GetWidth() > 2 * size ? a.GetSize().GetWidth() - 2 * size : 0, a.GetSize().GetHeight() > 2 * size ? a.GetSize().GetHeight() - 2 * size : 0));
	}

	Rectangle Rectangle::Contract(Rectangle a, Size1D size, Direction direction)
	{
		// TODO: size check
		if (direction == DIRECTION_HORIZONTAL)
		{
			return Rectangle(a.GetPosition() + Point(size, 0), Size2D(a.GetSize().GetWidth() > 2 * size ? a.GetSize().GetWidth() - 2 * size : 0, 0));
		}
		else if (direction == DIRECTION_VERTICAL)
		{
			return Rectangle(a.GetPosition() + Point(0, size), Size2D(0, a.GetSize().GetHeight() > 2 * size ? a.GetSize().GetHeight() - 2 * size : 0));
		}
		else throw ApiUsageException(GG_STR("Invalid direction."));
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


