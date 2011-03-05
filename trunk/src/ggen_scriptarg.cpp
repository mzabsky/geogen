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

#include <assert.h>

#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen.h"
#include "ggen_scriptarg.h"

bool GGen_ScriptArg::SetValue(int new_value){
	assert(GGen::GetInstance()->GetStatus() == GGEN_READY_TO_GENERATE);
	
	if(new_value < min_value) {
		value = min_value;
		return false;
	}

	if(new_value > max_value) {
		value = max_value;
		return false;
	}

	if((new_value - min_value) % step_size != 0){
		value = new_value - (new_value - min_value) % step_size;
		return false;
	}

	value = new_value;

	return true;
}

void GGen_AddIntArg(const GGen_String& name, const GGen_String& label, const GGen_String& description, int default_value, int min_value, int max_value, int step_size){
	GGen_Script_Assert(GGen::GetInstance()->GetStatus() == GGEN_LOADING_MAP_INFO);
	
	GGen_ScriptArg arg;

	arg.type = GGEN_INT;

	arg.name = name;
	arg.label = label;
	arg.description = description;
	arg.default_value = default_value;
	arg.min_value = min_value;
	arg.max_value = max_value;
	arg.step_size = step_size;

	arg.value = default_value;

	GGen::GetInstance()->args.push_back(arg);
};

void GGen_AddBoolArg(const GGen_String& name, const GGen_String& label, const GGen_String& description, bool default_value){
	GGen_Script_Assert(GGen::GetInstance()->GetStatus() == GGEN_LOADING_MAP_INFO);

	GGen_ScriptArg arg;

	arg.type = GGEN_BOOL;

	arg.name = name;
	arg.label = label;
	arg.description = description;
	arg.default_value = default_value ? 1 : 0;
	arg.min_value = 0;
	arg.max_value = 1;
	arg.step_size = 1;

	arg.value = default_value;

	GGen::GetInstance()->args.push_back(arg);
};

void GGen_AddEnumArg(const GGen_String& name, const GGen_String& label, const GGen_String& description, int default_value, const GGen_String& options){
	GGen_Script_Assert(GGen::GetInstance()->GetStatus() == GGEN_LOADING_MAP_INFO);

	GGen_ScriptArg arg;

	arg.type = GGEN_ENUM;

	arg.name = name;
	arg.label = label;
	arg.description = description;
	arg.default_value = default_value;

	GGen_Script_Assert(options.length() > 0);

	int from = 0;

	for(uint16 i = 0; i <= options.length(); i++){
		if(i == options.length() || options[i] == GGen_Const_String(';')){
			arg.options.push_back(options.substr(from, i - from));

			from = ++i;
		}
	}

	GGen_Script_Assert((signed) arg.options.size() > default_value);

	arg.min_value = 0;
	arg.max_value = arg.options.size() - 1;
	arg.step_size = 1;

	arg.value = default_value;

	GGen::GetInstance()->args.push_back(arg);
}


int GGen_GetArgValue(const GGen_String& name){
	GGen_Script_Assert(GGen::GetInstance()->GetStatus() == GGEN_GENERATING);

	for(unsigned i = 0; i < GGen::GetInstance()->args.size(); i++){
		if(GGen::GetInstance()->args[i].name == name){
			return GGen::GetInstance()->args[i].value;
		}
	}

	GGen::GetInstance()->ThrowMessage(GGen_Const_String("Current map doesn't support requested argument."), GGEN_ERROR);

	return 0;
}

