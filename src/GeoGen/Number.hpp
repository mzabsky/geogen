#pragma once

#include "String.hpp"

namespace geogen
{
	typedef double Number;
	typedef short Height;

	const Number NUMBER_PI = 3.14159265358979323846;

	Number StringToNumber(String const& str);
	bool IsNumberInt(Number n);
	int NumberToInt(Number n);
}