#include <string>
#include <vector>
#include <sstream>
#include <iostream>

namespace geogen
{
	typedef char Char;
	typedef std::string String;
	typedef std::stringstream StringStream;
	typedef std::iostream IOStream;

	String FormatString(String str, std::vector<String> arguments);
}