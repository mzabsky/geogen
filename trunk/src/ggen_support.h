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
#include "sqplus.h"

// hide stupid "sprintf is deprecated function, use our better alternative" MSVS warnings
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long long int64;
typedef unsigned long long uint64;

/* Type definitons. */
typedef int16 GGen_Height; /* Holds height for one cell. Minimum and maximum height are depend on this type. Must allow negative values. */
typedef int32 GGen_ExtHeight; /* Extended height value used for some calculations. Must be able to hold a value of 2 * GGen_Height * GGen_Height. */
typedef int64 GGen_ExtExtHeight; /* Even more extended height value used for some calculations. Must be able to hold a value of 2 * GGen_ExtHeight * GGen_ExtHeight. */
typedef uint16 GGen_Size; /* Size of the data array or size of an interval of coordinates in one dimension. Determines maximum dimensions of a data array. Must not allow negative values. */
typedef uint32 GGen_TotalSize; /* Total count of elements in the data array, must be able to hold at least GGen_Size * GGen_Size. Must not allow negative values. */
typedef GGen_Size GGen_Coord; /* Coordinate in one dimension of the data array. Should be the same as GGen_Size. */
typedef int32 GGen_CoordOffset; /* Coordinate offset relative to an offset value. Must allow negative values. */
typedef GGen_TotalSize GGen_Index; /* Coordinate in linearized data array. Should be the same as GGen_TotalSize. */
typedef uint32 GGen_Distance; /* Distance between two coordinates. Must hold 2 * GGen_Coord * GGen_Coord. */

#define GGEN_INVALID_HEIGHT -32768
#define GGEN_MIN_HEIGHT -32767
#define GGEN_MAX_HEIGHT 32767

#define GGEN_UNRELATIVE_CAP 255

#define MAX(a,b) (a > b? a: b) 
#define MIN(a,b) (a < b? a: b) 
#define ABS(a) (a < 0? -(a): a) 

// Custom assertion handler. Invoke messaage callback and shut down the script execution.
#define GGen_Script_Assert(_Expression) {if(!(_Expression)) {\
	char* as_buf = new char[400]; \
	as_buf[0] = '\0'; \
	as_buf = strcat(as_buf, "Assertion in function "); \
	as_buf = strcat(as_buf, __FUNCTION__); \
	as_buf = strcat(as_buf, " failed: "); \
	as_buf = strcat(as_buf, #_Expression); \
	ggen_current_object->ThrowMessage(as_buf, GGEN_ERROR, -1); \
	delete as_buf; \
	throw SquirrelError(); \
}}

enum GGen_Normalization_Mode{
	GGEN_ADDITIVE,
	GGEN_SUBSTRACTIVE
};

enum GGen_Overflow_Mode{
	GGEN_CYCLE,
	GGEN_DISCARD,
	GGEN_DISCARD_AND_FILL
};

enum GGen_Direction{
	GGEN_HORIZONTAL,
	GGEN_VERTICAL,
};

enum GGen_Message_Level{
	GGEN_MESSAGE = 0,
	GGEN_NOTICE = 1,
	GGEN_WARNING = 2,
	GGEN_ERROR = 3
};

enum GGen_Arg_Type{
	GGEN_BOOL,
	GGEN_INT,
	GGEN_ENUM
};

enum GGen_Voronoi_Noise_Mode{
	GGEN_RIDGES,
	GGEN_BUBBLES
};

template <class T>
T GGen_Random(T min, T max){
	double random = (double)rand() / (double) RAND_MAX;
	T output = min + (T) (random * (double)(max - min));
	return output;
}

inline int GGen_log2(int x){
	static double base = log10((double) 2);
	return (int16) (log10((double) x)/ base);
}

char* GGen_ToCString(const char* string);
char* GGen_ToCString(const wchar_t* string);
char* GGen_ToCString(const SqPlus::sq_std_string &string);


#endif