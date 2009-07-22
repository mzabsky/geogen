/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
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

#include "sqplus.h"

class GGen{
public:
	GGen();
	~GGen();

	void (*message_callback) (char* message, GGen_Message_Level, int line, int column);
	void (*return_callback) (char* name, int16* map, int width, int height);
	void (*post_callback) (GGen_Data_2D* map);

	GGen_ScriptArg* args[255];
	uint8 num_args;

	uint16 output_width, output_height;

	void ThrowMessage(char* message, GGen_Message_Level level, int line = -1, int column = -1);
	void ThrowMessage(const wchar_t* message, GGen_Message_Level level, int line = -1, int column = -1);

	void SetMessageCallback( void (*message_callback) (char* message, GGen_Message_Level, int line, int column));
	virtual void SetReturnCallback( void (*return_callback) (char* name, int16* map, int width, int height) );

	virtual bool SetScript(const char* script) = 0;
	virtual char* GetInfo(char* label) = 0;
	virtual int GetInfoInt(char* label) = 0;
	virtual GGen_ScriptArg** LoadArgs();
	virtual int16* Generate() = 0;

};