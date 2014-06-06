#include "StringUtils.hpp"

using namespace std;
using namespace geogen;
using namespace utils;

vector<String> geogen::utils::StringToLines(String const& str)
{
	StringStream ss(str);
	vector<String> codeLines;
	String currentLine;
	while (getline(ss, currentLine, '\n'))
	{
		codeLines.push_back(currentLine);
	}

	return codeLines;
}