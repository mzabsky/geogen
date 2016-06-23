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

#include "NumberFromEnumFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/IncorrectTypeException.hpp"
#include "NumberTypeDefinition.hpp"
#include "EnumTypeDefinition.hpp"
#include "InvalidEnumValueException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* NumberFromEnumFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	this->CheckArguments(location, 1, arguments);

	if (arguments[0]->IsStaticObject())
	{
		throw IncorrectTypeException(GGE2101_IncorrectNativeFunctionArgumentType, location, GG_STR("Enum"), GG_STR("Type"));
	}
	else if (!arguments[0]->GetType()->IsEnumType())
	{
		throw IncorrectTypeException(GGE2101_IncorrectNativeFunctionArgumentType, location, GG_STR("Enum"), arguments[0]->GetType()->GetName());
	}

	NumberObject* numberValue = dynamic_cast<NumberObject*>(arguments[0]);

	return numberTypeDefinition->CreateInstance(vm, numberValue->GetValue());
}