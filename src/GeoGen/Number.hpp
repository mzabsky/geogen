#include <string>

namespace geogen
{
	typedef double Number;
	typedef short Height;

	Number StringToNumber(std::string const& str);
	bool IsNumberInt(Number n);
	int NumberToInt(Number n);
}