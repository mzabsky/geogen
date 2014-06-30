#include "Interval.hpp"

using namespace geogen;

namespace geogen
{
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
}