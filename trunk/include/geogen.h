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

//#include "../external/squirrel/sqplus.h"

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;


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

class GGen{
public:
	GGen();
	~GGen();

	void (*message_callback) (char* message, GGen_Message_Level, int line, int column);
	void (*return_callback) (char* name, int16* map, int width, int height);
	//void (*post_callback) (GGen_Data_2D* map);
	void (*progress_callback) (int current_progress, int max_progress);

	GGen_ScriptArg* args[255];
	uint8 num_args;

	uint16 output_width, output_height;

	uint32 max_progress, current_progress;

	void ThrowMessage(char* message, GGen_Message_Level level, int line = -1, int column = -1);
	void ThrowMessage(const wchar_t* message, GGen_Message_Level level, int line = -1, int column = -1);

	void SetMessageCallback( void (*message_callback) (char* message, GGen_Message_Level, int line, int column));
	void SetReturnCallback( void (*return_callback) (char* name, int16* map, int width, int height) );
	void SetProgressCallback( void (*return_callback) (int current_progress, int max_progress));
	
	virtual bool SetScript(const char* script) = 0;
	virtual char* GetInfo(char* label) = 0;
	virtual int GetInfoInt(char* label) = 0;
	virtual GGen_ScriptArg** LoadArgs();
	virtual int16* Generate() = 0;

};

class GGen_Squirrel: public GGen{
public:	
	GGen_Squirrel();
	~GGen_Squirrel();

	virtual bool SetScript(const char* script);
	virtual char* GetInfo(char* label);
	virtual int GetInfoInt(char* label);
	virtual int16* Generate();
};
