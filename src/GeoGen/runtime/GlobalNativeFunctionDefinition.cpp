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