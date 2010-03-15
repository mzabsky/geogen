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
#include <string>

using namespace std;

#ifdef _UNICODE
	#define GGEN_UNICODE
#endif

#ifdef UNICODE
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

enum GGen_Status{
	GGEN_NO_SCRIPT,
	GGEN_SCRIPT_LOADED,
	GGEN_READY_TO_GENERATE,
	GGEN_LOADING_MAP_INFO,
	GGEN_GENERATING,
};

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

	GGen_Status GetStatus();

	void ThrowMessage(const GGen_String& message, GGen_Message_Level level, int line = -1, int column = -1);
	
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
};

class GGen_Squirrel: public GGen{
public:	
	GGen_Squirrel();
	~GGen_Squirrel();

	virtual bool SetScript(const GGen_String& script);
	virtual GGen_String GetInfo(const GGen_String& label);
	virtual int GetInfoInt(const GGen_String& label);
	virtual short* Generate();

	void* presetTarget;
};
