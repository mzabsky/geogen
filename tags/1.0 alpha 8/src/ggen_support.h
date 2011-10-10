/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

	GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GGEN_SUPPORT
#define GGEN_SUPPORT

#include "math.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <limits.h>

/** 
 * @file ggen_support.h File containing basic typedefs and enums used in rest of the GeoGen.
 **/

// hide stupid "sprintf is deprecated function, use our better alternative" MSVS warnings
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

using namespace std;

/* Define DLL export macro */
#ifdef _USRDLL
	#define GGEN_EXPORT __declspec(dllexport)
#else
	#define GGEN_EXPORT  
#endif

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long long int64;
typedef unsigned long long uint64;

/* Type definitions. */

/**
 * Height value for one tile. Value range: <-32768, 32767>.
 **/
typedef int16 GGen_Height;

/**
 * @internal Extended height value used for some calculations. Must be able to hold a value of 2 * GGen_Height * GGen_Height.
 **/
typedef int32 GGen_ExtHeight;

/**
 * @internal Even more extended height value used for some calculations. Must be able to hold a value of 2 * GGen_ExtHeight * GGen_ExtHeight.
 **/
typedef int64 GGen_ExtExtHeight;

/**
 * Size (in one dimension) of a data array or of an coordinate interval. Value range: <0, 65535>.
 * @internal Must not allow negative values.
 **/
typedef uint16 GGen_Size;

/**
 * @internal Total count of elements in a data array. Must not allow negative values.
 **/
typedef uint32 GGen_TotalSize; /* Total count of elements in the data array, must be able to hold at least GGen_Size * GGen_Size. Must not allow negative values. */

/**
 * Coordinate into a data array. Leftmost/topmost element has coordinate 0.
 * @internal Must not allow negative values. Should be the same as GGen_Size.
 **/
typedef uint16 GGen_Coord;

/**
 * Relative coordinate offset. Positive values mean offset to left/bottom, negative values offset to right/top.
 * @internal Must allow negative values.
 **/
typedef int32 GGen_CoordOffset;

/**
 * @internal Coordinate in linearized data array. Should be the same as GGen_TotalSize.
 **/
typedef GGen_TotalSize GGen_Index;

/**
 * Value representing euclidean distance between two points.
 * @internal Must hold 2 * GGen_Coord * GGen_Coord.
 **/
typedef uint32 GGen_Distance;

/**
 * Value representing invalid tile.
 **/
#define GGEN_INVALID_HEIGHT -32768

/**
 * Minimum reachable height.
 **/
#define GGEN_MIN_HEIGHT -32767

/**
 * Maximum reachable height.
 **/
#define GGEN_MAX_HEIGHT 32767

/**
 * Minimum map width/height(/length for 1D).
 **/
#define GGEN_MIN_MAP_SIZE 2

/**
 * Maximum number of points in a path object.
 **/
#define GGEN_MAX_PATH_LENGTH 20000

#define GGEN_UNRELATIVE_CAP GGEN_MAX_HEIGHT

#define MAX(a,b) ((a) > (b) ? (a) : (b)) 
#define MIN(a,b) ((a) < (b) ? (a) : (b)) 
#define ABS(a) ((a) < 0 ? -(a): (a)) 

#ifdef _UNICODE
	#ifndef GGEN_UNICODE 
		#define GGEN_UNICODE
	#endif
#endif

#ifdef GGEN_UNICODE
	typedef wchar_t GGen_Char;
	#define WIDEN(x) L ## x
	#define GGen_Const_String(a) WIDEN(a)
	#define GGen_Cin		wcin
	#define GGen_Cout		wcout
	#define	GGen_Strcmp		wcscmp
	#define GGen_Sprintf	swprintf
	#define GGen_Strlen		wcslen
	#define GGen_Strtod		wcstod
	#define GGen_Strtol		wcstol
	#define GGen_Atoi		_wtoi
	#define GGen_Strtoul	wcstoul
	#define GGen_Vsprintf	vswprintf
	#define GGen_Strstr		wcsstr
	#define GGen_Isspace	iswspace
	#define GGen_Isdigit	iswdigit
	#define GGen_Isxdigit	iswxdigit
	#define GGen_Isalpha	iswalpha
	#define GGen_Iscntrl	iswcntrl
	#define GGen_Isalnum	iswalnum
	#define GGen_Printf		wprintf
#else
	typedef char GGen_Char;
	#define GGen_Const_String(a) a
	#define GGen_Cin		cin	
	#define GGen_Cout		cout
	#define	GGen_Strcmp		strcmp
	#define GGen_Sprintf	sprintf
	#define GGen_Strlen		strlen
	#define GGen_Strtod		strtod
	#define GGen_Strtol		strtol
	#define GGen_Atoi		atoi
	#define GGen_Strtoul	strtoul
	#define GGen_Vsprintf	vsprintf
	#define GGen_Strstr		strstr
	#define GGen_Isspace	isspace
	#define GGen_Isdigit	isdigit
	#define GGen_Isxdigit	isxdigit
	#define GGen_Isalpha	iscntrl
	#define GGen_Iscntrl	isalpha
	#define GGen_Isalnum	isalnum
	#define GGen_Printf		printf
#endif 

typedef GGEN_EXPORT basic_string<GGen_Char> GGen_String;
typedef basic_stringstream<GGen_Char> GGen_StringStream;

struct GGen_ScriptAssertException{};

// Custom assertion handler. Invoke messaage callback and shut down the script execution.
// GCC can't convert the __FUNCTION__ macro to unicode property using the L## syntax -> it has to be done on runtime
#ifdef GGEN_UNICODE
	#define GGen_Script_Assert(_Expression) {if(!(_Expression)) {\
		GGen_String as_buf; \
		as_buf += GGen_Const_String("Assertion in function "); \
		int as_len = strlen(__FUNCTION__);\
		GGen_Char* as_buf2 = new GGen_Char[as_len + 1];\
		mbstowcs(as_buf2, __FUNCTION__, as_len);\
		as_buf2[as_len] = GGen_Const_String('\0');\
		as_buf += as_buf2;\
		delete [] as_buf2;\
		as_buf += GGen_Const_String(" failed: "); \
		as_buf += GGen_Const_String(#_Expression); \
		GGen::GetInstance()->ThrowMessage(as_buf, GGEN_ERROR, -1); \
		throw GGen_ScriptAssertException(); \
		}}
#else
	#define GGen_Script_Assert(_Expression) {if(!(_Expression)) {\
		GGen_String as_buf; \
		as_buf += GGen_Const_String("Assertion in function "); \
		as_buf += GGen_Const_String(__FUNCTION__); \
		as_buf += GGen_Const_String(" failed: "); \
		as_buf += GGen_Const_String(#_Expression); \
		GGen::GetInstance()->ThrowMessage(as_buf, GGEN_ERROR, -1);\
		throw GGen_ScriptAssertException(); \
		}}
#endif

#define GGen_Script_Error(message) \
	GGen::GetInstance()->ThrowMessage(GGen_Const_String(message), GGEN_ERROR, -1); \
	throw GGen_ScriptAssertException();


/**
 * Normalization mode (for GGen_Data_1D::Normalize and GGen_Data_2D::Normalize) defining behavior for too steep slopes.
 **/
enum GGen_Normalization_Mode{
	GGEN_ADDITIVE, //!< Too steep slopes will be neutralized by increasing the values (going from the hill-top).
	GGEN_SUBSTRACTIVE //!< Too steep slopes will be neutralized by decreasing the values (going from the bottom).
};

/**
 * Overflow mode (for GGen_Data_1D::Shift and GGen_Data_2D::Shift) defining behavior for values, which would be shifted outside the array and for values which would be shifted from outside in.
 **/
enum GGen_Overflow_Mode{
	GGEN_CYCLE, //!< Values will be cycled through the boundary (disappearing values will appear on the opposite side).
	GGEN_DISCARD,  //!< Disappearing values will be discarded, newly appearing values will be set to 0.
	GGEN_DISCARD_AND_FILL  //!< Disappearing values will be discarded, newly appearing values will be set to the closest valid value (along the shift axis).
};

/**
 * 2D axial directions.
 **/
enum GGen_Direction{
	GGEN_HORIZONTAL, //!< The operation will be done along the horizontal (X) axis.
	GGEN_VERTICAL, //!< The operation will be done along the vertical (Y) axis.
};

enum GGen_Message_Level{
	GGEN_MESSAGE = 0,
	GGEN_NOTICE = 1,
	GGEN_WARNING = 2,
	GGEN_ERROR = 3
};

/**
 * Script argument type.
 **/
enum GGen_Arg_Type{
	GGEN_BOOL, //!< Boolean value (yes/no) represented as integer from 0 to 1.
	GGEN_INT, //!< Integer in given range.
	GGEN_ENUM //!< Enumerated value (exactly one of listed values) represented as integer from 0 to (option count - 1).
};

/**
 * Voronoi noise type.
 **/
enum GGen_Voronoi_Noise_Mode{
	GGEN_RIDGES, //!< The resulting noise will have appearance of an array of crystals. 
	GGEN_BUBBLES //!< The resulting noise will have appearance of negative of an array of spherical bubbles.
};

/**
 * Arithmetic comparison operators.
 **/
enum GGen_Comparison_Mode{
	GGEN_EQUAL_TO, //!< Compared value is equal to threshold value. 
	GGEN_NOT_EQUAL_TO, //!< Compared value is not equal to threshold value.
	GGEN_LESS_THAN, //!< Compared value is less than threshold value.
	GGEN_GREATER_THAN, //!< Compared value is greater than threshold value.
	GGEN_LESS_THAN_OR_EQUAL_TO, //!< Compared value is less than or equal to threshold value.
	GGEN_GREATER_THAN_OR_EQUAL_TO, //!< Compared value is greater than or equal to threshold value.
};

/**
 * Outline mode.
 **/
enum GGen_Outline_Mode{
	GGEN_INSIDE, //!< The border line will be just inside the bordered area (the last cells matching the condition while "looking" from inside the bordered area).
	GGEN_OUTSIDE //!< The border line will be just outside the bordered area (the first cells not matching the condition while "looking" from inside the bordered area).
};

/**
 * Generator status
 */
enum GGen_Status{ 
	GGEN_NO_SCRIPT, //!< No script has been inserted yet, no actions but SetScript and setting map constraints are allowed. SetScript will rise the status to SCRIPT_LOADED.
	GGEN_SCRIPT_LOADED, //!< Script has been successfully loaded, work with map info is now allowed (LoadArgs can be called only now). LoadArgs will rise the status to READY_TO_GENERATE.
	GGEN_READY_TO_GENERATE, //!< Script is ready to be executed.
	GGEN_LOADING_MAP_INFO, //!< Map header is being mined for information, no work with map data of any kind is allowed.
	GGEN_GENERATING, //!< Script is being executed. All script actions but adding new arguments are allowed.
};

template <class T>
T GGen_Random(int min, int max){
	return min + (rand() % (int)(max - min + 1));
}

inline int GGen_log2(int x){
	static double base = log10((double) 2);
	return (int16) (log10((double) x)/ base);
}

#endif