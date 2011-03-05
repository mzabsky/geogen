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

using namespace ::SqPlus;

#include "ggen_support.h"
#include "ggen.h"



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


DECLARE_INSTANCE_TYPE(GeoGen::Data_1D)
DECLARE_INSTANCE_TYPE(GeoGen::Data_2D)
DECLARE_INSTANCE_TYPE(GeoGen::Amplitudes)
DECLARE_INSTANCE_TYPE(GeoGen::Point)
DECLARE_INSTANCE_TYPE(GeoGen::Path)

namespace GeoGen{
	class GGEN_EXPORT SquirrelGenerator: public Generator{
	protected:
		list<void*> presets;
	public:	
		SquirrelGenerator();
		virtual ~SquirrelGenerator();

		virtual bool SetScript(const String& script);
		virtual String GetInfo(const String& label);
		virtual int GetInfoInt(const String& label);
		virtual int16* Generate();
	
		virtual void RegisterPreset(Data_1D* preset, const String& label);
		virtual void RegisterPreset(Data_2D* preset, const String& label);
		virtual void RegisterPreset(Amplitudes* preset, const String& label);
	};
}

