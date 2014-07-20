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
}


