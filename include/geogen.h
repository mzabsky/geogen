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

#include "../external/squirrel/sqplus.h"

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

class GGen{
public:
	GGen();
	~GGen();

//private:
	void (*message_callback) (char* message, GGen_Message_Level, int line, int column);
	void (*return_callback) (char* name, int16* map);

	void ThrowMessage(char* message, GGen_Message_Level level, int line = 0, int column = 0);
	void ThrowMessage(const wchar_t* message, GGen_Message_Level level, int line = 0, int column = 0);


public:
	void SetMessageCallback( void (*message_callback) (char* message, GGen_Message_Level, int line, int column));
	virtual void SetReturnCallback( void (*return_callback) (char* name, int16* map) );

	virtual bool SetScript(const char* script) = 0;
	virtual char* GetInfo(char* label) = 0;
	virtual int GetInfoInt(char* label) = 0;
	virtual bool GetNextOption() = 0;
	virtual int16* Generate(uint16 width, uint16 height) = 0;

};

class GGen_Squirrel: public GGen{
public:	
	//SquirrelObject sqScript;

	GGen_Squirrel();
	~GGen_Squirrel();

	virtual bool SetScript(const char* script);
	virtual char* GetInfo(char* label);
	virtual int GetInfoInt(char* label);
	virtual bool GetNextOption();
	virtual int16* Generate(uint16 width, uint16 height);
};
