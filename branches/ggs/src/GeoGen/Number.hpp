#pragma once

#include <limits>

#include "CodeLocation.hpp"
#include "String.hpp"
#include "Size.hpp"

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
}