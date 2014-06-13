#include <string>
#include <vector>
#include <sstream>
#include <iostream>

namespace geogen
{

#ifdef GEOGEN_ASCII
	typedef char Char;
	typedef std::string String;
	typedef std::stringstream StringStream;
	typedef std::iostream IOStream;	

	#define GG_STR(str) str
#else
	typedef wchar_t Char;
	typedef std::wstring String;
	typedef std::wstringstream StringStream;
	typedef std::wiostream IOStream;

	#ifndef WIDEN
		#define WIDEN(str) L ## str
	#endif
	#define GG_STR(str) WIDEN(str)
#endif	

	String FormatString(String str, std::vector<String> arguments);

	std::string StringToAscii(String str);
	String AnyStringToString(std::string str);

	std::wstring StringToWstring(String str);
	String AnyStringToString(std::wstring str);
}