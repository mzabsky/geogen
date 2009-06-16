/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef GGEN_SUPPORT
#define GGEN_SUPPORT

#include "sqplus.h"

// hide stupid "sprintf is deprecated function, use our better alternative" MSVS warnings
#define _CRT_SECURE_NO_WARNINGS

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

#define MAX(a,b) (a > b? a: b) 
#define MIN(a,b) (a < b? a: b) 

#define GGEN_INVALID_HEIGHT -32768
#define GGEN_MIN_HEIGHT -32767
#define GGEN_MAX_HEIGHT 32767

// Custom GGen_Script_Assertion handler. Invoke messaage callback and shut down the script execution.
#define GGen_Script_Assert(_Expression) {if(!(_Expression)) {ggen_current_object->ThrowMessage(#_Expression, GGEN_ERROR, -1); throw SquirrelError();} }

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

enum GGen_Angle{
	GGEN_0,
	GGEN_90,
	GGEN_180,
	GGEN_270
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

using namespace std;

template <class T>
T Random(T min, T max){
	double random = (double)rand() / (double) RAND_MAX;
	T output = min + (T) (random * (double)(max - min));
	return output;
}

inline int log2(int x){
	static double base = log10((double) 2);
	return (int16) (log10((double) x)/ base);
}

char* GGen_ToCString(const char* string);
char* GGen_ToCString(const wchar_t* string);
char* GGen_ToCString(const SqPlus::sq_std_string &string);


#endif