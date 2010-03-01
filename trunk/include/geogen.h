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

#pragma once

#include <vector>

#ifdef _UNICODE
	#define GGEN_UNICODE
#endif

#ifdef GGEN_UNICODE
	typedef wchar_t GGen_Char;
	#define GGEN_WIDEN(x) L ## x
	#define GGen_Const_String(a) GGEN_WIDEN(a)
	#define GGen_Cout		wcout
#else
	typedef char GGen_Char;
	#define GGen_Const_String(a) a
	#define GGen_Cout		cout
#endif

typedef basic_string<GGen_Char> GGen_String;

//#include "../external/squirrel/sqplus.h"

/*
typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long long int64;
typedef unsigned long long uint64;*/

/* enum GGen_Normalization_Mode{
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
 */
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

/* Type definitons. */
//typedef int16 GGen_Height; /* Holds height for one cell. Minimum and maximum height are depend on this type. Must allow negative values. */
//typedef int32 GGen_ExtHeight; /* Extended height value used for some calculations. Must be able to hold a value of 2 * GGen_Height * GGen_Height. */
//typedef int64 GGen_ExtExtHeight; /* Even more extended height value used for some calculations. Must be able to hold a value of 2 * GGen_ExtHeight * GGen_ExtHeight. */
//typedef uint16 GGen_Size; /* Size of the data array or size of an interval of coordinates in one dimension. Determines maximum dimensions of a data array. Must not allow negative values. */
//typedef uint32 GGen_TotalSize; /* Total count of elements in the data array, must be able to hold at least GGen_Size * GGen_Size. Must not allow negative values. */
//typedef GGen_Size GGen_Coord; /* Coordinate in one dimension of the data array. Should be the same as GGen_Size. */
//typedef int32 GGen_CoordOffset; /* Coordinate offset relative to an offset value. Must allow negative values. */
//typedef GGen_TotalSize GGen_Index; /* Coordinate in linearized data array. Should be the same as GGen_TotalSize. */
//typedef uint32 GGen_Distance; /* Distance between two coordinates. Must hold 2 * GGen_Coord * GGen_Coord. */

class GGen_ScriptArg{
public:
	int value;
	GGen_String name;
	GGen_String label;
	GGen_String description;
	GGen_Arg_Type type;
	int min_value;
	int max_value;
	int default_value;
	int step_size;
	vector<GGen_String> options;;

	bool SetValue(int new_value);
};

/*
class GGen_Amplitudes{
public:
	uint16* data;
	uint16 length;

	GGen_Amplitudes(uint16 max_feature_size);

	~GGen_Amplitudes(){ 
		//delete [] data; 
	}

	void AddAmplitude(uint16 feature_size, uint16 amplitude);
};

*/

class GGen{
protected: 
	static GGen* instance;

public:
	void (*message_callback) (const GGen_String& message, GGen_Message_Level, int line, int column);
	void (*return_callback) (const GGen_String& name, const short* map, int width, int height);
	void (*progress_callback) (int current_progress, int max_progress);

	vector<GGen_ScriptArg> args;

	unsigned short output_width, output_height;

	unsigned max_progress, current_progress;

	unsigned short max_width, max_height;
	unsigned short max_map_count;

	GGen();
	~GGen();

	static GGen* GetInstance();

	void ThrowMessage(const GGen_String& message, GGen_Message_Level level, int line = -1, int column = -1);
	//void ThrowMessage(const wchar_t* message, GGen_Message_Level level, int line = -1, int column = -1);

	void SetMessageCallback( void (*message_callback) (const GGen_String& message, GGen_Message_Level, int line, int column));
	void SetReturnCallback( void (*return_callback) (const GGen_String& name, const short* map, int width, int height) );
	void SetProgressCallback( void (*return_callback) (int current_progress, int max_progress));
	
	virtual bool SetScript(const GGen_String& script) = 0;
	virtual GGen_String GetInfo(const GGen_String& label) = 0;
	virtual int GetInfoInt(const GGen_String& label) = 0;
	virtual vector<GGen_ScriptArg>* LoadArgs();
	virtual short* Generate() = 0;
	
	void SetMaxWidth(unsigned short width);
	void SetMaxHeight(unsigned short height);
	void SetMaxMapCount(unsigned short count);

	/* Constraint getters and progress methods must be static to be exported as globals to Squirrel */
	static unsigned short GetMaxWidth();
	static unsigned short GetMaxHeight();
	static unsigned short GetMaxMapCount();

	static void InitProgress(unsigned max_progress);
	static void SetProgress(unsigned current_progress);
	static void IncreaseProgress();
	
	/*virtual void RegisterPreset(GGen_Data_1D* preset, char* label) = 0;
	virtual void RegisterPreset(GGen_Data_2D* preset, char* label) = 0;
	virtual void RegisterPreset(GGen_Amplitudes* preset, char* label) = 0;*/
};

class GGen_Squirrel: public GGen{
public:	
	GGen_Squirrel();
	~GGen_Squirrel();

	virtual bool SetScript(const GGen_String& script);
	virtual GGen_String GetInfo(const GGen_String& label);
	virtual int GetInfoInt(const GGen_String& label);
	virtual short* Generate();
	
	/*virtual void RegisterPreset(void* preset, char* label);
	virtual void RegisterPreset(void* preset, char* label);
	virtual void RegisterPreset(void* preset, char* label);*/
	
	void* presetTarget;
};
