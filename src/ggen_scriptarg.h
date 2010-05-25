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

/** 
 * @file ggen_scriptarg.h Contains functions working with script arguments.
 **/

#pragma once

/* Hide the "wstring needs to have dll-interface to be used by clients of class 'GGen_ScriptArg'" */
#pragma warning(disable: 4251)

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

/**
 * Defines a new integral script argument. Can be called only within script header.
 * @param name Unique internal name.
 * @param label Label (name presented to users).
 * @param description Detailed description.
 * @param default_value Default value.
 * @param min_value Minimum value.
 * @param max_value Maximum value.
 * @param step_size Step size (UI only - represents value change for various up/down arrows or sliders). This setting will not be enforced - user can be allowed to manually type in values between steps.
 **/
void GGen_AddIntArg(const GGen_String& name, const GGen_String& label, const GGen_String& description, int default_value, int min_value, int max_value, int step_size);

/**
 * Defines a new boolean script argument. Can be called only within script header.
 * @param name Unique internal name.
 * @param label Label (name presented to users).
 * @param description Detailed description.
 * @param default_value Default value.
 **/
void GGen_AddBoolArg(const GGen_String& name, const GGen_String& label, const GGen_String& description, bool default_value);

/**
 * Defines a new enumeration script argument. Can be called only within script header.
 * @param name Unique internal name.
 * @param label Label (name presented to users).
 * @param description Detailed description.
 * @param default_value Default value (index of default option starting at 0).
 * @param options Semicolon-separated list of options.
 **/
void GGen_AddEnumArg(const GGen_String& name, const GGen_String& label, const GGen_String& description, int default_value, const GGen_String& options);

/**
 * Returns value of one script argument. Can be called only within script body.
 * @param name Name of the argument.
 * @return Value of the argument. Always integral - boolean values are returned as 0/1 and enumeration values as index of the chosen option.
 **/
int GGen_GetArgValue(const GGen_String& name);