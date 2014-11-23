#include <cctype>

#include "String.hpp"
#include "InternalErrorException.hpp"
#include "ApiUsageException.hpp"

using namespace std;
using namespace geogen;

enum StringFormatterState
{
	STATE_CHARACTER,
	STATE_OPEN_BRACE,
	STATE_INDEX,
	STATE_FREE_CLOSING_BRACE
};

String geogen::FormatString(String str, std::vector<String> arguments) {
	StringStream ss;
	unsigned index = 0;

	StringFormatterState state = STATE_CHARACTER;
	for (unsigned i = 0; i < str.length(); i++)
	{
		Char c = str[i];
		
		switch (state)
		{
		case STATE_CHARACTER:
			if (c == '{')
			{
				state = STATE_OPEN_BRACE;
			}
			else if (c == '}')
			{
				state = STATE_FREE_CLOSING_BRACE;
			}
			else
			{
				ss << c;
			}

			break;
		case STATE_OPEN_BRACE:
			if (isdigit(c))
			{
				index = c - '0';
				state = STATE_INDEX;
			}
			else if (c == '{')
			{
				ss << '{';
				state = STATE_CHARACTER;
			}
			else
			{
				throw ApiUsageException(GG_STR("Invalid format string."));
			}

			break;
		case STATE_INDEX:
			if (isdigit(c))
			{
				index = index * 10 + (unsigned)(c - '0');
			}
			else if (c == '}')
			{
				if (index < arguments.size())
				{
					ss << arguments[index];
					index = 0;
				}
				else
				{
					throw ApiUsageException(GG_STR("Invalid format string."));
				}

				state = STATE_CHARACTER;
			}
			else
			{
				throw ApiUsageException(GG_STR("Invalid format string."));
			}

			break;
		case STATE_FREE_CLOSING_BRACE:
			if (c == '}')
			{
				ss << '}';
				state = STATE_CHARACTER;
			}
			else
			{
				throw ApiUsageException(GG_STR("Invalid format string."));
			}

			break;
		default:
			throw InternalErrorException(GG_STR("Invalid string formatter state."));
			break;
		}		
	}

	if (state != STATE_CHARACTER)
	{
		throw InternalErrorException(GG_STR("Invalid string formatter state."));
	}

	return ss.str();
}

string geogen::StringToAscii(String str)
{
	return string(str.begin(), str.end());
}

wstring geogen::StringToWstring(String str)
{
	return wstring(str.begin(), str.end());
}

String geogen::AnyStringToString(string str)
{
	return String(str.begin(), str.end());
}

String geogen::AnyStringToString(wstring str)
{
	return String(str.begin(), str.end());
}

int geogen::StringToInt(String str)
{
#ifndef GGEN_WCHAR
	wchar_t* end;
	return wcstol(str.c_str(), &end, 10);
#else
	return atoi(str.c_str());
#endif
}

String geogen::IntToString(int n)
{
	StringStream ss;
	ss << n;
	return ss.str();
}