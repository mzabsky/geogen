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

#include "../external/squirrel/sqplus.h"

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long long int64;
typedef unsigned long long uint64;

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

class GGen_ScriptArg{
public:
	int value;
	char* name;
	char* label;
	char* description;
	GGen_Arg_Type type;
	int min_value;
	int max_value;
	int default_value;
	int step_size;
	char** options;
	int num_options;
	bool strict_steps;

	bool SetValue(int new_value);

	/*static void GGen_AddIntArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, int default_value, int min_value, int max_value, int step_size);
	static void GGen_AddBoolArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, bool default_value);
	static void GGen_AddEnumArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, int default_value, const SqPlus::sq_std_string &options);*/
};

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

class GGen_Data_1D{
	public:
		GGen_Height* data;
		GGen_Size length;

		/* Constructors/destructors */
		GGen_Data_1D(GGen_Size length);
		GGen_Data_1D(GGen_Size length, GGen_Height value);
		GGen_Data_1D(GGen_Data_1D& victim);
		~GGen_Data_1D();

		/* Basic data I/O */
		GGen_Size GetLength();
		void SetValue(GGen_Coord x, GGen_Height value);
		void SetValueInRange(GGen_Coord from, GGen_Coord to, GGen_Height value);
		GGen_Height GetValue(GGen_Coord x);
		GGen_Height GetValue(GGen_Coord x, GGen_Size scale_to_length);

		/* Elementary artihmetic and logic operations */
		void Add(GGen_Height value);
		void Add(GGen_Data_1D* addend);
		void AddTo(GGen_Data_1D* addend, GGen_CoordOffset offset);
		void AddMasked(GGen_Height value, GGen_Data_1D* mask, bool relative);
		void AddMasked(GGen_Data_1D* addend, GGen_Data_1D* mask, bool relative);
		void Multiply(double value);
		void Multiply(GGen_Data_1D* factor);
		void Invert();
		void Scale(double ratio, bool scale_values);
		void ScaleTo(GGen_Size new_length, bool scale_values);
		void ScaleValuesTo(GGen_Height new_min, GGen_Height new_max);
		void Fill(GGen_Height value);
		void ResizeCanvas(GGen_Size new_length, GGen_CoordOffset new_zero);
		void Clamp(GGen_Height min, GGen_Height max);
		void Flip();
		GGen_Height Min();
		GGen_Height Max();
		void Shift(GGen_CoordOffset distance, GGen_Overflow_Mode mode);
		void Union(GGen_Data_1D* victim);
		void Intersection(GGen_Data_1D* victim);
		void Abs();
		
		/* Advanced operations with array data */
		void Monochrome(GGen_Height treshold);
		void Normalize(GGen_Normalization_Mode mode);
		void SlopeMap();
		void Gradient(GGen_Coord from, GGen_Coord to, GGen_Height from_value, GGen_Height to_value, bool fill_flat);
		void Noise(GGen_Size min_feature_size, GGen_Size max_feature_size, GGen_Amplitudes* amplitudes);
		void Smooth(GGen_Distance radius);
		void Flood(double water_amount);
};

class GGen_Data_2D{
	public:
		GGen_Height* data;
		GGen_Size width;
		GGen_Size height;
		GGen_TotalSize length;

		/* Constructors/destructors */
		GGen_Data_2D(GGen_Size width, GGen_Size height);
		GGen_Data_2D(GGen_Size width, GGen_Size height, GGen_Height value);
		GGen_Data_2D(GGen_Data_2D& victim);
		~GGen_Data_2D();

		/* Basic data I/O */
		GGen_Size GetWidth();
		GGen_Size GetHeight();
		GGen_Index GetLength();
		void SetValue(GGen_Coord x, GGen_Coord y, GGen_Height value);
		void SetValueInRect(GGen_Coord x1, GGen_Coord y1, GGen_Coord x2, GGen_Coord y2, GGen_Height value);
		GGen_Height GetValue(GGen_Coord x, GGen_Coord y);
		GGen_Height GetValue(GGen_Coord x, GGen_Coord y, GGen_Size scale_to_width, GGen_Size scale_to_height);

		/* Elementary data manipulation */
		void Add(GGen_Height value);
		void Add(GGen_Data_2D* addend);
		void AddTo(GGen_Data_2D* addend, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y);
		void AddMasked(GGen_Data_2D* addend, GGen_Data_2D* mask, bool relative);
		void AddMasked(GGen_Height value, GGen_Data_2D* mask, bool relative);
		void Multiply(double factor);
		void Multiply(GGen_Data_2D* factor);
		void Invert();
		void Scale(double ratio, bool scale_values);
		void ScaleTo(GGen_Size new_width, GGen_Size new_height, bool scale_values);
		void ScaleValuesTo(GGen_Height new_min, GGen_Height new_max);
		void ResizeCanvas(GGen_Size new_width, GGen_Size new_height, GGen_CoordOffset new_zero_x, GGen_CoordOffset new_zero_y);
		void Fill(GGen_Height value);
		void Clamp(GGen_Height min, GGen_Height max);
		GGen_Height Min();
		GGen_Height Max();
		void Intersection(GGen_Data_2D* victim);
		void IntersectionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y);
		void Union(GGen_Data_2D* victim);
		void UnionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y);
		void Combine(GGen_Data_2D* victim, GGen_Data_2D* mask, bool relative);
		void ReplaceValue(GGen_Height needle, GGen_Height replace);
		void Abs();

		void Shift(GGen_Data_1D* profile, GGen_Direction direction, GGen_Overflow_Mode mode);
		void Project(GGen_Data_1D* profile, GGen_Direction direction);
		GGen_Data_1D* GetProfile(GGen_Direction direction, GGen_Coord coordinate);

		/* Advanced data manipulation */
		void Gradient(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Height from_value, GGen_Height to_value, bool fill_outside);
		void Gradient(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Data_1D* pattern, bool fill_outside);
		void RadialGradient(GGen_Coord center_x, GGen_Coord center_y, GGen_Coord radius, GGen_Height min, GGen_Height max, bool fill_outside);
		void RadialGradient(GGen_Coord center_x, GGen_Coord center_y, GGen_Distance radius, GGen_Data_1D* pattern, bool fill_outside);
		void Noise(GGen_Size min_feature_size, GGen_Size max_feature_size, GGen_Amplitudes* amplitudes);
		void Smooth(GGen_Distance radius, GGen_Direction direction);
		void Smooth(GGen_Distance radius);
		void Flood(double water_amount);
		void Pattern(GGen_Data_2D* pattern);
		void Monochrome(GGen_Height treshold);
		void SelectValue(GGen_Height value);
		void SlopeMap();
		void Scatter(bool relative);
		void TransformValues(GGen_Data_1D* profile, bool relative);
		void Normalize(GGen_Normalization_Mode mode);
		void Normalize(GGen_Direction direction, GGen_Normalization_Mode mode);
		
		/* Matrix operations */
		void Transform(double a11, double a12, double a21, double a22, bool preserve_size);
		void Rotate(int32 angle, bool preserve_size);
		void Shear(int32 horizontal_shear, int32 vertical_shear, bool preserve_size);
		void Flip(GGen_Direction direction);


		/* External communication functions */
		void ReturnAs(const SqPlus::sq_std_string &name);
};

class GGen{
public:
	GGen();
	~GGen();

	void (*message_callback) (char* message, GGen_Message_Level, int line, int column);
	void (*return_callback) (char* name, const int16* map, int width, int height);
	void (*progress_callback) (int current_progress, int max_progress);


	GGen_ScriptArg* args[255];
	uint8 num_args;


	uint16 output_width, output_height;

	uint32 max_progress, current_progress;

	void ThrowMessage(char* message, GGen_Message_Level level, int line = -1, int column = -1);
	void ThrowMessage(const wchar_t* message, GGen_Message_Level level, int line = -1, int column = -1);

	void SetMessageCallback( void (*message_callback) (char* message, GGen_Message_Level, int line, int column));
	void SetReturnCallback( void (*return_callback) (char* name, const int16* map, int width, int height) );
	void SetProgressCallback( void (*return_callback) (int current_progress, int max_progress));
	
	virtual bool SetScript(const char* script) = 0;
	virtual char* GetInfo(char* label) = 0;
	virtual int GetInfoInt(char* label) = 0;
	virtual GGen_ScriptArg** LoadArgs();
	virtual int16* Generate() = 0;
	
	static void InitProgress(uint32 max_progress);
	static void SetProgress(uint32 current_progress);
	static void IncreaseProgress();
	
	virtual void RegisterPreset(GGen_Data_1D* preset, char* label) = 0;
	virtual void RegisterPreset(GGen_Data_2D* preset, char* label) = 0;
	virtual void RegisterPreset(GGen_Amplitudes* preset, char* label) = 0;
};

class GGen_Squirrel: public GGen{
public:	
	GGen_Squirrel();
	~GGen_Squirrel();

	virtual bool SetScript(const char* script);
	virtual char* GetInfo(char* label);
	virtual int GetInfoInt(char* label);
	virtual int16* Generate();
	
	virtual void RegisterPreset(GGen_Data_1D* preset, char* label);
	virtual void RegisterPreset(GGen_Data_2D* preset, char* label);
	virtual void RegisterPreset(GGen_Amplitudes* preset, char* label);
	
	SquirrelObject* presetTarget;
};
