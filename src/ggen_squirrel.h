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

#include "sqplus.h"
#include "ggen_support.h"
#include "ggen.h"

using namespace SqPlus;


static void printFunc(HSQUIRRELVM v,const SQChar * s,...);

#define SQ_REL_PATH "../external/squirrel_lib/"

#ifdef UNICODE
#define LIB  "U.lib"
#else
#define LIB  ".lib"
#endif

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib,SQ_REL_PATH "squirrelD" LIB)
#pragma comment(lib,SQ_REL_PATH "sqstdlibD" LIB)
#ifdef USE_REMOTE_DEBUGGER
#pragma comment(lib,SQ_REL_PATH "sqdbglibD" LIB)
#endif
#pragma comment(lib,SQ_REL_PATH "sqplusD" LIB)
#else // Release
#pragma comment(lib,SQ_REL_PATH "squirrel" LIB)
#pragma comment(lib,SQ_REL_PATH "sqstdlib" LIB)
#ifdef USE_REMOTE_DEBUGGER
#pragma comment(lib,SQ_REL_PATH "sqdbglib" LIB)
#endif
#pragma comment(lib,SQ_REL_PATH "sqplus" LIB)
#endif
#endif





DECLARE_INSTANCE_TYPE(GGen_Data_1D)
DECLARE_INSTANCE_TYPE(GGen_Data_2D)
DECLARE_INSTANCE_TYPE(GGen_Amplitudes)

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

