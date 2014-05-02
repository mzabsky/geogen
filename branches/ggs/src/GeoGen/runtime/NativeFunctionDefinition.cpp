#include "NativeFunctionDefinition.hpp"
#include "VirtualMachine.hpp"
#include "NumberOfArgumentsException.hpp"
#include "TypeDefinition.hpp"
#include "ManagedObject.hpp"
#include "IncorrectTypeException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

void NativeFunctionDefinition::Call(CodeLocation location, VirtualMachine* vm, unsigned numberOfArguments) const
{
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

void NativeFunctionDefinition::CheckArguments(CodeLocation location, vector<TypeDefinition const*> expectedTypes, vector<ManagedObject*> actualArguments) const
{
	CheckArguments(location, expectedTypes.size(), actualArguments);

	for (vector<ManagedObject*>::size_type i = 0; i < actualArguments.size(); i++)
	{
		if (actualArguments[i]->IsStaticObject())
		{
			ErrorCode errorCode = this->GetFunctionType() == FUNCTION_TYPE_FUNCTION ? GGE2101_IncorrectNativeFunctionArgumentType : GGE2102_IncorrectOperandType;

			throw IncorrectTypeException(errorCode, location, expectedTypes[i]->GetName(), "Static");			
		}

		if (actualArguments[i]->GetType() != expectedTypes[i])
		{
			ErrorCode errorCode = this->GetFunctionType() == FUNCTION_TYPE_FUNCTION ? GGE2101_IncorrectNativeFunctionArgumentType : GGE2102_IncorrectOperandType;

			throw IncorrectTypeException(errorCode, location, expectedTypes[i]->GetName(), actualArguments[i]->GetType()->GetName());
		}
	}
}

void NativeFunctionDefinition::CheckArguments(CodeLocation location, unsigned expectedArgumentCount, vector<ManagedObject*> actualArguments) const
{
	if (actualArguments.size() != expectedArgumentCount)
	{
		throw NumberOfArgumentsException(location, expectedArgumentCount, actualArguments.size());
	}
}