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