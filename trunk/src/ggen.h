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

#pragma once

// system and SDL headers
#include <stdio.h>   
#include <stdlib.h> 
#include <string>
//#include <SDL/SDL.h> 
#include <math.h>
#include <time.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#include "ggen_support.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"

#include "sqplus.h"

class GGen{
public:
	GGen();
	~GGen();

//private:
	void (*callback) (char* message, GGen_Message_Level, int line, int column);

	void ThrowMessage(char* message, GGen_Message_Level level, int line = 0, int column = 0);
	void ThrowMessage(const wchar_t* message, GGen_Message_Level level, int line = 0, int column = 0);


public:
	void SetMessageCallback( void (*callback) (char* message, GGen_Message_Level, int line, int column));

	virtual bool SetScript(const char* script) = 0;
	virtual void GetProperties() = 0;
	virtual bool GetNextOption() = 0;
	virtual int16* Generate(uint16 width, uint16 height) = 0;

};