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