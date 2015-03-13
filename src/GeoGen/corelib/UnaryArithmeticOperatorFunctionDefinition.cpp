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

#include "UnaryArithmeticOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "DivisionByZeroException.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

UnaryArithmeticOperatorFunctionDefinition* UnaryArithmeticOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case UNARY_PLUS: return new UnaryArithmeticOperatorFunctionDefinition(GG_STR("+un"), CallOperatorUnaryPlus);
	case UNARY_MINUS: return new UnaryArithmeticOperatorFunctionDefinition(GG_STR("-un"), CallOperatorUnaryMinus);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* UnaryArithmeticOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	NumberObject* a = dynamic_cast<NumberObject*>(arguments[0]);

	Number result = this->function(location, a->GetValue());

	ManagedObject* returnObject = numberTypeDefinition->CreateInstance(vm, result);
	return returnObject;
}

Number UnaryArithmeticOperatorFunctionDefinition::CallOperatorUnaryPlus(CodeLocation location, Number a)
{
	return a;
}

Number UnaryArithmeticOperatorFunctionDefinition::CallOperatorUnaryMinus(CodeLocation location, Number a)
{
	return -a;
}