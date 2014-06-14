#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

namespace geogen
{
	/// <summary>
	/// Character encoding in 1-byte character mode. 
	/// </summary>
	/*enum Encoding8bit
	{
		SINGLE_BYTE,
		UTF8LE
	};*/

#ifndef GEOGEN_WCHAR
	typedef char Char;
	typedef std::string String;
	typedef std::stringstream StringStream;
	typedef std::ifstream IFStream;
	typedef std::ofstream OFStream;
	typedef std::ostream OStream;
	typedef std::istream IStream;
	typedef std::iostream IOStream;	

	static OStream& Cout = std::cout;
	static IStream& Cin = std::cin;

	#define GG_STR(str) str
#else
	typedef wchar_t Char;
	typedef std::wstring String;
	typedef std::wstringstream StringStream;
	typedef std::wifstream IFStream;
	typedef std::wofstream OFStream;
	typedef std::wostream OStream;
	typedef std::wistream IStream;
	typedef std::wiostream IOStream;	

	static OStream& Cout = std::wcout;
	static IStream& Cin = std::wcin;

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