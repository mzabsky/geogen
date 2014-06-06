#include <string>
#include <vector>

#include "CodeLocation.hpp"

namespace geogen
{
	typedef std::string String;
	typedef std::stringstream StringStream;

	String FormatString(String str, std::vector<String> arguments);
}