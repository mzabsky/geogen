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

#include "ArrayFrontBackFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;


ArrayFrontBackFunctionDefinition* ArrayFrontBackFunctionDefinition::Create(Method method, runtime::TypeDefinition const* owningType)
{
	switch (method)
	{
	case FRONT:
		return new ArrayFrontBackFunctionDefinition(GG_STR("Front"), method, owningType);
	case BACK:
		return new ArrayFrontBackFunctionDefinition(GG_STR("Back"), method, owningType);
	default: throw InternalErrorException(GG_STR("Unknown method."));
	}
}

ManagedObject* ArrayFrontBackFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	this->CheckArguments(location, 0, arguments);

	ArrayObject* thisArray = dynamic_cast<ArrayObject*>(instance);

	switch (this->method)
	{
	case FRONT:
		return thisArray->Front(vm, location);
	case BACK:
		return thisArray->Back(vm, location);
	default:
		throw InternalErrorException(GG_STR("Unknown method."));
	}
}