#include <cfenv>

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
		throw ApiUsageException("Height overflow.");
	}

	return h;
}

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
}