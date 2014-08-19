#pragma once

#include <vector>

#include "../String.hpp"

namespace geogen
{
	namespace utils
	{
		std::vector<String> StringToLines(String const& str);
		String FormatFileSize(unsigned sizeInBytes);
	}
}