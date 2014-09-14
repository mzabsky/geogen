#pragma once

#include <vector>

#include "../String.hpp"

namespace geogen
{
	namespace utils
	{
		/// Converts a string into a collection of lines.
		/// @param str The string.
		/// @return A collection of lines;
		std::vector<String> StringToLines(String const& str);

		/// Format a number as a file size (with appropriate file size units).
		/// @param sizeInBytes The size in bytes.
		/// @return The formatted file size.
		String FormatFileSize(unsigned sizeInBytes);
	}
}