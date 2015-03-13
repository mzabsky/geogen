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

#include "GlobalNativeFunctionDefinition.hpp"
#include "VirtualMachine.hpp"
#include "NumberOfArgumentsException.hpp"
#include "TypeDefinition.hpp"
#include "ManagedObject.hpp"
#include "IncorrectTypeException.hpp"
#include "..\InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

void GlobalNativeFunctionDefinition::Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const
{
	if (instance != NULL)
	{
		throw new InternalErrorException(GG_STR("Static functions cannot be instance methods."));
	}

	ObjectStack& objectStack = vm->GetObjectStack();
	CallStack& callStack = vm->GetCallStack();

	// Pop the arguments from the stack	
	objectStack.CheckSize(numberOfArguments);

	vector<ManagedObject*> arguments;
	for (unsigned i = 0; i < numberOfArguments; i++)
	{
		arguments.push_back(objectStack.Top());
		objectStack.Top()->AddRef();
		objectStack.Pop(vm);
	}

	reverse(arguments.begin(), arguments.end());

	ManagedObject* returnValue = this->CallNative(location, vm, arguments);

	callStack.Pop();

	for (unsigned i = 0; i < numberOfArguments; i++)
	{
		arguments[i]->RemoveRef(vm->GetMemoryManager());
	}

	objectStack.Push(location, returnValue);
}

void GlobalNativeFunctionDefinition::Serialize(IOStream& stream) const
{
	stream << GG_STR("<native>") << endl;
}