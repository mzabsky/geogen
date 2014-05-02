#include <iostream>
#include <fstream>
#include <sstream>

#include "StringUtils.hpp"

using namespace std;
using namespace geogen;
using namespace utils;

vector<string> geogen::utils::StringToLines(std::string const& str)
{
	stringstream ss(str);
	vector<string> codeLines;
	string currentLine;
	while (getline(ss, currentLine, '\n'))
	{
		codeLines.push_back(currentLine);
	}

	return codeLines;
}