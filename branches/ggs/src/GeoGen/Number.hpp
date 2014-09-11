#pragma once

#include <limits>

#include "CodeLocation.hpp"
#include "String.hpp"
#include "Size.hpp"
#include "Point.hpp"

namespace geogen
{
	/// A number. 
	typedef double Number;

	/// A height in range HEIGHT_MIN, HEIGHT_MAX.
	typedef short Height;

	/// A scale ratio.
	typedef double Scale;

	/// The minimum height.
	const Height HEIGHT_MIN = -32767;

	/// The maximum height.
	const Height HEIGHT_MAX = 32767;

	/// Mathematical constant Pi.
	const Number NUMBER_PI = 3.14159265358979323846;

	/// Converts String to Number.
	/// @param str The string.
	/// @return The number. 0 if the conversion failed.
	Number StringToNumber(String const& str);

	/// Determines whether a Number is an integer.
	/// @param n The number.
	/// @return true if the number is an integer, false otherwise.
	bool IsNumberInt(Number n);
	
	/// Converts a Number to an integer.
	/// @param n The number.
	/// @return The converted integer.
	int NumberToInt(Number n);

	/// Attempts to convert a Number to a Height. The number is expected to be in range <-1, 1>.
	/// @param n The Number to convert.
	/// @param out The conversion result.
	/// @return true if the conversion was successful, false otherwise.
	bool TryNumberToHeight(Number n, Height& out);

	/// Attempts to convert a Number to Size1D. The number is expected to be in range SIZE_MIN, SIZE_MAX.
	/// @param n The Number to convert.
	/// @param out The conversion result.
	/// @return true if the conversion was successful, false otherwise.
	bool TryNumberToSize(Number n, Size1D& out);

	/// Converts a Number to a Height. The number is expected to be in range <-1, 1>.
	/// @param n The Number to convert.
	/// @return The converted Height.
	Height NumberToHeight(Number n);

	/// Returns sign of a numeric value.
	/// @tparam T Numeric type.
	/// @param val The value.
	/// @return -1 if the value was less than 0, 0 if it was 0 and 1 if it was greater than 0.
	template <typename T> int Sign(T val) {
		return (T(0) < val) - (val < T(0));
	}

	/// Starts a zone of checked floating point math.
	void RuntimeMathCheckInit();
	
	/// Closes a zone of checked floating point math and throws an exception if an error was detected. Any operation that would result in infinity or NaN is considered to be erroneous.
	void RuntimeMathCheck(CodeLocation location);

	/// Linear interpolation between two values on two 1D coordinates. The two coordinates must not be the same. 
	/// @param fromCoord From coordinate (x0).
	/// @param toCoord To coordinate (x1).
	/// @param fromHeight From height (y0).
	/// @param toHeight To height (y1).
	/// @param point The point (x).
	/// @return An interpolated height.
	/// @see http://en.wikipedia.org/wiki/Linear_interpolation
	static inline Height Lerp(Coordinate fromCoord, Coordinate toCoord, Height fromHeight, Height toHeight, double point)
	{
		return Height(fromHeight + (toHeight - fromHeight) * (point - fromCoord) / (toCoord - fromCoord));
	}

	/// Rounds a number to the next integer away from zero.
	/// @param x The number.
	/// @return The rounded number.
	static inline double RoundAway(double x)
	{
		return x > 0 ? ceil(x) : floor(x);
	}

}