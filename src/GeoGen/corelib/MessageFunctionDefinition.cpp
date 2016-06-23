/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include "MessageFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/StringTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "MathDefinitionRangeException.hpp"
#include "../runtime\UserErrorException.hpp"
#include "StringFormatException.hpp"
#include "../String.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

MessageFunctionDefinition* MessageFunctionDefinition::Create(Function function)
{
	switch (function)
	{
	case PRINT:
		return new MessageFunctionDefinition(GG_STR("Print"), function);
	case TRIGGER_ERROR:
		return new MessageFunctionDefinition(GG_STR("TriggerError"), function);
	default: throw InternalErrorException(GG_STR("Unknown method."));
	}
}

ManagedObject* MessageFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	StringTypeDefinition const* stringTypeDefinition = dynamic_cast<StringTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("String")));

	vector<TypeDefinition const*> expectedParameterTypes;
	expectedParameterTypes.push_back(stringTypeDefinition);
	if (arguments.size() > 1){
		for (unsigned i = 1; i < arguments.size(); i++)
		{
			expectedParameterTypes.push_back(arguments[i]->GetType());
		}
	}

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedParameterTypes, arguments);

	String formatStr = dynamic_cast<StringObject*>(arguments[0])->GetValue();
	vector<String> strArgs;
	if (arguments.size() > 1){
		for (unsigned i = 1; i < arguments.size(); i++)
		{
			strArgs.push_back(arguments[i]->GetStringValue());
		}
	}
	
	String str;
	try
	{
		str = FormatString(formatStr, strArgs);
	}	
	catch (ApiUsageException)
	{
		throw StringFormatException(location);
	}

	switch (this->function)
	{
	case PRINT:
		if (vm->GetScriptMessageHandler() != NULL)
		{
			vm->GetScriptMessageHandler()(vm, location, str, formatStr, strArgs);
		}		
		break;
	case TRIGGER_ERROR:
		throw UserErrorException(location, str, formatStr, strArgs);
		break;
	default:
		throw InternalErrorException(GG_STR("Unknown method."));
	}

	return vm->GetNull();
}