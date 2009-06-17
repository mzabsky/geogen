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

#include "sqplus.h"


#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen.h"
#include "ggen_scriptarg.h"

extern GGen* ggen_current_object;

bool GGen_ScriptArg::SetValue(int new_value){
	//bool return_value;

	if(new_value < min_value) {
		value = min_value;
		return false;
	}

	if(new_value > max_value) {
		value = max_value;
		return false;
	}

	if(strict_steps && (new_value - min_value) % step_size != 0){
		value = new_value - (new_value - min_value) % step_size;
		return false;
	}

	value = new_value;

	return true;
}

void GGen_ScriptArg::GGen_AddIntArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, int default_value, int min_value, int max_value, int step_size){
	GGen_ScriptArg* arg = new GGen_ScriptArg();

	arg->type = GGEN_INT;

	arg->name = GGen_ToCString(name);
	arg->label = GGen_ToCString(label);
	arg->description = GGen_ToCString(description);
	arg->default_value = default_value;
	arg->min_value = min_value;
	arg->max_value = max_value;
	arg->step_size = step_size;
	//arg->strict_steps = strict_steps;
	arg->options = NULL;
	arg->num_options = 0;

	arg->value = default_value;

	ggen_current_object->args[ggen_current_object->num_args] = arg;
	ggen_current_object->num_args++;

	// add the argument to the argument array
	/*if(ggen_current_object->num_args == 0){
		ggen_current_object->args = new GGen_ScriptArg*[1];

		ggen_current_object->args[0] = arg;
		ggen_current_object->num_args = 1;
	}
	else{
		realloc(ggen_current_object->args, ggen_current_object->num_args + 1);
		ggen_current_object->args[ggen_current_object->num_args] = arg;
		ggen_current_object->num_args++;
	}*/
};

void GGen_ScriptArg::GGen_AddBoolArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, bool default_value){
	GGen_ScriptArg* arg = new GGen_ScriptArg();

	arg->type = GGEN_BOOL;

	arg->name = GGen_ToCString(name);
	arg->label = GGen_ToCString(label);
	arg->description = GGen_ToCString(description);
	arg->default_value = default_value ? 1 : 0;
	arg->min_value = 0;
	arg->max_value = 1;
	arg->step_size = 1;
	//arg->strict_steps = strict_steps;
	arg->options = NULL;
	arg->num_options = 0;

	arg->value = default_value;

	ggen_current_object->args[ggen_current_object->num_args] = arg;
	ggen_current_object->num_args++;
};

void GGen_ScriptArg::GGen_AddEnumArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, int default_value, const SqPlus::sq_std_string &options){
	GGen_ScriptArg* arg = new GGen_ScriptArg();

	arg->type = GGEN_ENUM;

	arg->name = GGen_ToCString(name);
	arg->label = GGen_ToCString(label);
	arg->description = GGen_ToCString(description);
	arg->default_value = default_value;

	char* options_c = GGen_ToCString(options);

	int len = strlen(options_c);
	int from = 0;
	int option_i = 0;
	arg->options = new char*[255];

	for(uint8 i = 0; i <= len; i++){
		if(i == len || options_c[i] == ';'){
			arg->options[option_i] = new char[i - from + 1];
			strncpy(arg->options[option_i],options_c + from, i - from);
			arg->options[option_i][i - from] = '\0';

			from = ++i;
			option_i++;
		}
	}

	delete options_c;

	arg->num_options = option_i;

	arg->min_value = 0;
	arg->max_value = option_i - 1;
	arg->step_size = 1;
	//arg->strict_steps = strict_steps;


	arg->value = default_value;

	ggen_current_object->args[ggen_current_object->num_args] = arg;
	ggen_current_object->num_args++;
}


int GGen_GetParam(const SqPlus::sq_std_string &name){
	char* buf = GGen_ToCString(name);

	for(uint8 i = 0; i < ggen_current_object->num_args; i++){
		if(strcmp(ggen_current_object->args[i]->name,buf) == 0){
			delete buf;
			return ggen_current_object->args[i]->value;
		}
	}

	ggen_current_object->ThrowMessage("Current map doesn't support requested argument.",GGEN_ERROR);
	return 0;
}

