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

#include <iomanip>

#include "StringUtils.hpp"

using namespace std;
using namespace geogen;
using namespace utils;

vector<String> geogen::utils::StringToLines(String const& str)
{
	StringStream ss(str);
	vector<String> codeLines;
	String currentLine;
	while (getline(ss, currentLine, (Char)'\n'))
	{
		codeLines.push_back(currentLine);
	}

	return codeLines;
}

String geogen::utils::FormatFileSize(unsigned sizeInBytes)
{
	StringStream ss;
	if (sizeInBytes < 1024 * 1.5)
	{
		ss << sizeInBytes << " B";
	}
	else if (sizeInBytes < 1024 * 1024 * 1.5)
	{
		ss << std::fixed << std::setprecision(2) << (sizeInBytes / 1024.) << " KB";
	}
	else if (sizeInBytes < 1024 * 1024 * 1024 * 1.5)
	{
		ss << std::fixed << std::setprecision(2) << (sizeInBytes / (1024. * 1024.)) << " MB";
	}
	else
	{
		ss << std::fixed << std::setprecision(2) << (sizeInBytes / (1024. * 1024. * 1024.)) << " GB";
	}

	return ss.str();
}