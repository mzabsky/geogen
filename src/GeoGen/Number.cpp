#include "String.hpp"
#include "Number.hpp"

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
}
