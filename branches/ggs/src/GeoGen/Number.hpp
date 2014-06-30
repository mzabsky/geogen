#pragma once

#include <limits>

#include "String.hpp"

namespace geogen
{
	typedef double Number;
	typedef short Height;

	const Height HEIGHT_MIN = -32767;
	const Height HEIGHT_MAX = 32767;

	const Number NUMBER_PI = 3.14159265358979323846;

	Number StringToNumber(String const& str);
	bool IsNumberInt(Number n);
	int NumberToInt(Number n);

	bool TryNumberToHeight(Number n, Height& out);
	Height NumberToHeight(Number n);
}