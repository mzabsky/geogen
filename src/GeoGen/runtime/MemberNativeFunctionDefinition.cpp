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

#include "MemberNativeFunctionDefinition.hpp"
#include "ManagedObject.hpp"
#include "../InternalErrorException.hpp"
#include "VirtualMachine.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

void MemberNativeFunctionDefinition::Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const
{
	if (instance == NULL)
	{
		throw InternalErrorException(GG_STR("Instance functions must be called on an instance."));
	}

	runtime::ObjectStack& objectStack = vm->GetObjectStack();
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

	if (instance->GetType() != this->GetOwningTypeDefinition())
	{
		throw InternalErrorException(GG_STR("Method called on instance of incorrect type."));
	}

	if (this->GetMethodType() == METHOD_TYPE_INSTANCE && instance->IsStaticObject())
	{
		throw InternalErrorException(GG_STR("Can't call instance method on static object."));
	}
	else if (this->GetMethodType() == METHOD_TYPE_STATIC && !instance->IsStaticObject())
	{
		throw InternalErrorException(GG_STR("Can't call static method on an instance."));
	}

	ManagedObject* returnValue = this->CallNative(location, vm, instance, arguments);

	callStack.Pop();

	for (unsigned i = 0; i < numberOfArguments; i++)
	{
		arguments[i]->RemoveRef(vm->GetMemoryManager());
	}

	objectStack.Push(location, returnValue);
}

void MemberNativeFunctionDefinition::Serialize(IOStream& stream) const
{
	stream << GG_STR("<native>") << endl;
}