#include <string>
#include <sstream>

#include "Number.hpp"

namespace geogen 
{
	Number StringToNumber(std::string const& str)
	{
		std::istringstream i(str);
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
