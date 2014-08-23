#pragma once

#include <limits>

#include "CodeLocation.hpp"
#include "String.hpp"
#include "Size.hpp"
#include "Point.hpp"

namespace geogen
{
	typedef double Number;
	typedef short Height;
	typedef double Scale;

	const Height HEIGHT_MIN = -32767;
	const Height HEIGHT_MAX = 32767;

	const Number NUMBER_PI = 3.14159265358979323846;

	Number StringToNumber(String const& str);
	bool IsNumberInt(Number n);
	int NumberToInt(Number n);

	bool TryNumberToHeight(Number n, Height& out);
	bool TryNumberToSize(Number n, Size1D& out);
	Height NumberToHeight(Number n);

	void RuntimeMathCheckInit();
	void RuntimeMathCheck(CodeLocation location);

	/// Linear interpolation between to values on two 1D coordinates (see http://en.wikipedia.org/wiki/Linear_interpolation).
	/// @param fromCoord  From coordinate (x0).
	/// @param toCoord    To coordinate (x1).
	/// @param fromHeight From height (y0).
	/// @param toHeight   To height (y1).
	/// @param point	  The point (x).
	/// @return An interpolated height.
	static inline Height Lerp(Coordinate fromCoord, Coordinate toCoord, Height fromHeight, Height toHeight, double point)
	{
		return Height(fromHeight + (toHeight - fromHeight) * (point - fromCoord) / (toCoord - fromCoord));
	}

	static inline double RoundAway(double x)
	{
		return x > 0 ? ceil(x) : floor(x);
	}

}