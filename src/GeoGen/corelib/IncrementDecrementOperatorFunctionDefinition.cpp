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

#include "IncrementDecrementOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "NumberTypeDefinition.hpp"
#include "ReferenceTypeDefinition.hpp"
#include "ReferenceObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "DivisionByZeroException.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/NumberOverflowException.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;

IncrementDecrementOperatorFunctionDefinition* IncrementDecrementOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case PLUS_PLUS_PRE: return new IncrementDecrementOperatorFunctionDefinition(GG_STR("++pre"), op);
	case PLUS_PLUS_POST: return new IncrementDecrementOperatorFunctionDefinition(GG_STR("++post"), op);
	case MINUS_MINUS_PRE: return new IncrementDecrementOperatorFunctionDefinition(GG_STR("--pre"), op);
	case MINUS_MINUS_POST: return new IncrementDecrementOperatorFunctionDefinition(GG_STR("--post"), op);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* IncrementDecrementOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	ReferenceTypeDefinition const* referenceTypeDefinition = dynamic_cast<ReferenceTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("<Reference>")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(referenceTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	ReferenceObject* reference = dynamic_cast<ReferenceObject*>(arguments[0]);

	ManagedObjectHolder referencedObjectHolder;
	ManagedObject* referencedObject = reference->GetReferencedObject(location, vm);
	this->CheckArgument(vm, location, numberTypeDefinition, referencedObject, referencedObjectHolder);

	Number number = dynamic_cast<NumberObject*>(referencedObject)->GetValue();
	Number resultNumber;
	Number returnNumber;

	switch (op)
	{
	case PLUS_PLUS_PRE: resultNumber = number + 1; returnNumber = resultNumber; break;
	case PLUS_PLUS_POST: resultNumber = number + 1; returnNumber = number; break;
	case MINUS_MINUS_PRE: resultNumber = number - 1; returnNumber = resultNumber; break;
	case MINUS_MINUS_POST: resultNumber = number - 1; returnNumber = number; break;
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}

	if (resultNumber == numeric_limits<Number>::infinity() || resultNumber == -numeric_limits<Number>::infinity())
	{
		throw NumberOverflowException(location);
	}

	reference->SetReferencedObject(location, vm, numberTypeDefinition->CreateInstance(vm, resultNumber));

	ManagedObject* returnObject = numberTypeDefinition->CreateInstance(vm, returnNumber);
	return returnObject;
}