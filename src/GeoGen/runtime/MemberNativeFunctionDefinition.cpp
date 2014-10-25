#include "MemberNativeFunctionDefinition.hpp"
#include "ManagedObject.hpp"
#include "..\InternalErrorException.hpp"
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