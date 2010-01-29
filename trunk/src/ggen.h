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

#include "ggen_support.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"
#include "ggen_scriptarg.h"

class GGen;

class GGen{
protected: 
	static GGen* instance;

public:
	void (*message_callback) (char* message, GGen_Message_Level, int line, int column);
	void (*return_callback) (char* name, const int16* map, int width, int height);
	void (*progress_callback) (int current_progress, int max_progress);


	GGen_ScriptArg* args[255];
	uint8 num_args;

	uint16 output_width, output_height;

	uint32 max_progress, current_progress;

	GGen();
	~GGen();

	static GGen* GetInstance();

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
	
	/*void SetMaxWidth(GGen_Size width);
	void SetMaxHeight(GGen_Size height);
	void SetMaxMapCount(uint16 count);*/

	

	static void InitProgress(uint32 max_progress);
	static void SetProgress(uint32 current_progress);
	static void IncreaseProgress();
	
	virtual void RegisterPreset(GGen_Data_1D* preset, char* label) = 0;
	virtual void RegisterPreset(GGen_Data_2D* preset, char* label) = 0;
	virtual void RegisterPreset(GGen_Amplitudes* preset, char* label) = 0;
};