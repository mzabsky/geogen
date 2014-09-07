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

#ifdef DOXYGEN
	/// Switches the entire library to wide char mode, if defined.
	#define GEOGEN_WCHAR 
	#undef GEOGEN_WCHAR 
#endif

#ifndef GEOGEN_WCHAR
	/// A character in GeoGen string. Defined as wchar_t, if GEOGEN_WCHAR is defined.
	typedef char Char;
	/// A string type used to represent string throughout the library. Defined as std::wstring, if GEOGEN_WCHAR is defined.
	typedef std::string String;
	/// A string stream type used to represent string throughout the library. Defined as std::wstringstream, if GEOGEN_WCHAR is defined.
	typedef std::stringstream StringStream;
	/// An input file stream. Defined as std::wifstream, if GEOGEN_WCHAR is defined.
	typedef std::ifstream IFStream;
	/// An output file stream. Defined as std::wofstream, if GEOGEN_WCHAR is defined.
	typedef std::ofstream OFStream;
	/// An input file stream. Defined as std::wostream, if GEOGEN_WCHAR is defined.
	typedef std::ostream OStream;
	/// An input file stream. Defined as std::wistream, if GEOGEN_WCHAR is defined.
	typedef std::istream IStream;
	/// An input file stream. Defined as std::wiostream, if GEOGEN_WCHAR is defined.
	typedef std::iostream IOStream;	

	/// Standard output. Defined as std::wcout, if GEOGEN_WCHAR is defined.
	static OStream& Cout = std::cout;
	/// Standard input. Defined as std::wcin, if GEOGEN_WCHAR is defined.
	static IStream& Cin = std::cin;

	/// Makes a string literal char* or wchar_t* based on whether GEOGEN_WCHAR is defined.
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

	/// Replaces formatting marks in a string with appropriate formatting arguments.
	/// @param str The string with formatting marks.
	/// @param arguments The formatting arguments.
	/// @return The formatted string.
	String FormatString(String str, std::vector<String> arguments);

	/// Converts String to an ASCII string (std::string<char>).
	/// @param str The string.
	/// @return The converted string.
	std::string StringToAscii(String str);

	/// Converts any string to String.
	/// @param str The string.
	/// @return The converted string.
	String AnyStringToString(std::string str);
	
	/// Converts String to an ASCII string (std::string<wchar>).
	/// @param str The string.
	/// @return The converted string.
	std::wstring StringToWstring(String str);

	/// Converts any string to String.
	/// @param str The string.
	/// @return The converted string.
	String AnyStringToString(std::wstring str);

	/// Converts String to an integer.
	/// @param str The string.
	/// @return The converted integer.
	int StringToInt(String str);

	/// Converts integer to String.
	/// @param n The integer.
	/// @return The converted string.
	String IntToString(int n);
}