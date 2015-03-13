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

#include "Interval.hpp"
#include "InternalErrorException.hpp"

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

	if (a.GetLength() == 0)
	{
		returnIntervalStart = b.GetStart();
		returnIntervalLength = b.GetLength();
	}
	else if (b.GetLength() == 0)
	{
	}
	else if (b.GetStart() < a.GetStart())
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

Interval Interval::FromRectangle(Rectangle rect, Direction direction)
{
	if (direction == DIRECTION_HORIZONTAL)
	{
		return Interval(rect.GetPosition().GetX(), rect.GetSize().GetWidth());		
	}
	else if (direction == DIRECTION_VERTICAL)
	{
		return Interval(rect.GetPosition().GetY(), rect.GetSize().GetHeight());
	}
	else
	{
		throw InternalErrorException(GG_STR("Invalid direction."));
	}
}

Interval Interval::Expand(Interval interval, Size1D distance)
{
	return Interval(interval.GetStart() - distance, interval.GetLength() + 2 * distance);
}