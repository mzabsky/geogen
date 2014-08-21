#include "Interval.hpp"

using namespace geogen;

bool Interval::Contains(Interval other) const
{
	return other.start >= this->start && other.end <= this->end;
}

bool Interval::Contains(Coordinate x) const
{
	return x >= this->start && x <= this->end;
}

Interval Interval::Combine(Interval a, Interval b)
{
	Coordinate returnIntervalStart = a.GetStart();
	Coordinate returnIntervalLength = a.GetLength();

	if (b.GetStart() < a.GetStart())
	{
		returnIntervalLength += a.GetStart() - b.GetStart();
		returnIntervalStart = b.GetStart();
	}

	if (b.GetLength() > a.GetLength())
	{
		returnIntervalLength = b.GetLength();
	}

	return Interval(returnIntervalStart, returnIntervalLength);
}

Interval Interval::Intersect(Interval a, Interval b)
{
	Coordinate returnIntervalStart = a.GetStart();
	Size1D returnIntervalLength = a.GetLength();

	if (a.GetStart() < b.GetStart())
	{
		returnIntervalLength += b.GetStart() - a.GetStart();
		returnIntervalStart = b.GetStart();
	}

	if (returnIntervalLength > b.GetLength())
	{
		returnIntervalLength = b.GetLength();
	}

	return Interval(returnIntervalStart, returnIntervalLength);
}

Interval Interval::FromRectangle(Rectangle rect, Orientation orientation)
{
	if (orientation == ORIENTATION_HORIZONTAL)
	{
		return Interval(rect.GetPosition().GetX(), rect.GetSize().GetWidth());		
	}
	else if (orientation == ORIENTATION_VERTICAL)
	{
		return Interval(rect.GetPosition().GetY(), rect.GetSize().GetHeight());
	}
}