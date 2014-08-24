#include <cfenv>

#include "String.hpp"
#include "Number.hpp"
#include "ApiUsageException.hpp"
#include "runtime/NumberOverflowException.hpp"
#include "runtime/NumberUnderflowException.hpp"

namespace geogen 
{
	Number StringToNumber(String const& str)
	{
		StringStream i(str);
		double x;
		if (!(i >> x))
			return 0;
		return x;
	}

	bool IsNumberInt(Number n)
	{
		return (int)n == n;
	}

	int NumberToInt(Number n)
	{
		return (int)n;
	}

	bool TryNumberToHeight(Number n, Height& out)
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

	bool TryNumberToSize(Number n, Size1D& out)
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


	Height NumberToHeight(Number n)
	{
		Height h = 0;
		if (!TryNumberToHeight(n, h))
		{
			throw ApiUsageException("Height overflow.");
		}

		return h;
	}

	void RuntimeMathCheckInit()
	{
		feclearexcept(FE_ALL_EXCEPT);
	}

	void RuntimeMathCheck(CodeLocation location)
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
}
