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
#include "ggen_support.h"

class GGEN_EXPORT GGen_ScriptArg{
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


void GGen_AddIntArg(const GGen_String& name, const GGen_String& label, const GGen_String& description, int default_value, int min_value, int max_value, int step_size);
void GGen_AddBoolArg(const GGen_String& name, const GGen_String& label, const GGen_String& description, bool default_value);
void GGen_AddEnumArg(const GGen_String& name, const GGen_String& label, const GGen_String& description, int default_value, const GGen_String& options);

int GGen_GetParam(const GGen_String& name);