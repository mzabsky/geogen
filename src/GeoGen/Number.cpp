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

//#include <cfenv>

#include "String.hpp"
#include "Number.hpp"
#include "ApiUsageException.hpp"
#include "runtime/NumberOverflowException.hpp"
#include "runtime/NumberUnderflowException.hpp"

using namespace geogen;

Number geogen::StringToNumber(String const& str)
{
	StringStream i(str);
	double x;
	if (!(i >> x))
		return 0;
	return x;
}

bool geogen::IsNumberInt(Number n)
{
	return (int)n == n;
}

int geogen::NumberToInt(Number n)
{
	return (int)n;
}

bool geogen::TryNumberToHeight(Number n, Height& out)
{
	if (n > 1 || n < -1)
	{
		return false;
	}
	/*else if (n > 0)
	{
		out = (Height)(n * HEIGHT_MAX);
		return true;
	}*/
	else {
		out = (Height)(n * HEIGHT_MAX);
		return true;
	}
}

bool geogen::TryNumberToScale(Number n, Scale& out)
{
	if (n > 10 || n < 0.1)
	{
		return false;
	}
	else {
		out = n;
		return true;
	}
}

bool geogen::TryNumberToSize(Number n, Size1D& out)
{
	if (n < 0)
	{
		return false;
	}
	else if (n > (Number)SIZE1D_MAX)
	{
		return false;
	}
	else {
		out = (Size1D)(n);
		return true;
	}
}


Height geogen::NumberToHeight(Number n)
{
	Height h = 0;
	if (!TryNumberToHeight(n, h))
	{
		throw ApiUsageException(GG_STR("Height overflow."));
	}

	return h;
}
/*
void geogen::RuntimeMathCheckInit()
{
	feclearexcept(FE_ALL_EXCEPT);
}

void geogen::RuntimeMathCheck(CodeLocation location)
{
	if (fetestexcept(FE_OVERFLOW))
	{
		throw runtime::NumberOverflowException(location);
	}
	else if (fetestexcept(FE_UNDERFLOW))
	{
		throw runtime::NumberUnderflowException(location);
	}
}*/