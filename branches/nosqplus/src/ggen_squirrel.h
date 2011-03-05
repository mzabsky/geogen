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

#include <list>

#include "../external/squirrel/sqplus.h"
#include "../external/squirrel/sqstdmath.h"
#include "../external/squirrel/sqstdio.h"
#include "../external/squirrel/sqstdmath.h"
#include "../external/squirrel/sqstdstring.h"
#include "../external/squirrel/sqstdaux.h"
#include "../external/squirrel/sqstdblob.h"
#include "../external/squirrel/sqstdsystem.h"

#include "ggen_support.h"
#include "ggen.h"

using namespace SqPlus;

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
DECLARE_INSTANCE_TYPE(GGen_Point)
DECLARE_INSTANCE_TYPE(GGen_Path)

class GGEN_EXPORT GGen_Squirrel: public GGen{
protected:
	list<void*> presets;
public:	
	bool isClassOpen;
	HSQUIRRELVM squirrelVM;

	GGen_Squirrel();
	virtual ~GGen_Squirrel();

	virtual bool SetScript(const GGen_String& script);
	virtual GGen_String GetInfo(const GGen_String& label);
	virtual int GetInfoInt(const GGen_String& label);
	virtual int16* Generate();
	
	virtual void RegisterPreset(GGen_Data_1D* preset, const GGen_String& label);
	virtual void RegisterPreset(GGen_Data_2D* preset, const GGen_String& label);
	virtual void RegisterPreset(GGen_Amplitudes* preset, const GGen_String& label);

private:
	template<typename Class, typename Method, int numConstructorParameters>
	void StartClass(GGen_String name, GGen_String constructorParameters);
	void EndClass();
	void RegisterMethod(GGen_String name, SQFUNCTION method, int16 numParameters, GGen_String parameters, void *userdata = NULL, int size = 0);
	template <typename Class, typename Method>
	void RegisterMethod(GGen_String name, Method method, int16 numParameters, GGen_String parameters);
	void RegisterConstans(GGen_String name, int value);
};