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

#include "sqplus.h"


#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen.h"
#include "ggen_scriptarg.h"

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

void GGen_AddIntArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, int default_value, int min_value, int max_value, int step_size){
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

	GGen::GetInstance()->args[GGen::GetInstance()->num_args] = arg;
	GGen::GetInstance()->num_args++;
};

void GGen_AddBoolArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, bool default_value){
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

	GGen::GetInstance()->args[GGen::GetInstance()->num_args] = arg;
	GGen::GetInstance()->num_args++;
};

void GGen_AddEnumArg(const SqPlus::sq_std_string &name, const SqPlus::sq_std_string &label, const SqPlus::sq_std_string &description, int default_value, const SqPlus::sq_std_string &options){
	GGen_ScriptArg* arg = new GGen_ScriptArg();

	arg->type = GGEN_ENUM;

	arg->name = GGen_ToCString(name);
	arg->label = GGen_ToCString(label);
	arg->description = GGen_ToCString(description);
	arg->default_value = default_value;

	char* options_c = GGen_ToCString(options);

	GGen_Script_Assert(strlen(options_c) > 0);

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

	GGen_Script_Assert(arg->num_options > default_value);

	arg->min_value = 0;
	arg->max_value = option_i - 1;
	arg->step_size = 1;
	//arg->strict_steps = strict_steps;


	arg->value = default_value;

	GGen::GetInstance()->args[GGen::GetInstance()->num_args] = arg;
	GGen::GetInstance()->num_args++;
}


int GGen_GetParam(const SqPlus::sq_std_string &name){
	char* buf = GGen_ToCString(name);

	for(uint8 i = 0; i < GGen::GetInstance()->num_args; i++){
		if(strcmp(GGen::GetInstance()->args[i]->name,buf) == 0){
			delete buf;
			return GGen::GetInstance()->args[i]->value;
		}
	}

	GGen::GetInstance()->ThrowMessage("Current map doesn't support requested argument.",GGEN_ERROR);

	return 0;
}

