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

#include "ArrayGetRefFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "ReferenceTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;

ManagedObject* ArrayGetRefFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	ReferenceTypeDefinition const* referenceTypeDefinition = dynamic_cast<ReferenceTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("<Reference>")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	this->CheckArguments(location, 1, arguments);

	ArrayObject* thisArray = dynamic_cast<ArrayObject*>(instance);

	// Do not store the key from the stack - it may be a temporary object that is just equivalent to the actual key
	ManagedObject* actualKey = thisArray->GetActualKey(vm, location, arguments[0]);	

	return referenceTypeDefinition->CreateArrayItemReferenceInstance(vm, thisArray, actualKey);
}