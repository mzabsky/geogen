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
#include "sqplus.h"

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

};

void GGen_AddIntArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, int default_value, int min_value, int max_value, int step_size);
void GGen_AddBoolArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, bool default_value);
void GGen_AddEnumArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, int default_value, const SqPlus::sq_std_string &options);


int GGen_GetParam(const SqPlus::sq_std_string &name);