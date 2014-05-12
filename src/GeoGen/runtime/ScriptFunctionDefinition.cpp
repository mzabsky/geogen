#include "ScriptFunctionDefinition.hpp"
#include "VirtualMachine.hpp"
#include "..\InternalErrorException.hpp"
#include "NumberOfArgumentsException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

void ScriptFunctionDefinition::Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const
{
	if (instance != NULL)
	{
		throw new InternalErrorException("Script functions cannot be instance methods.");
	}

	if (numberOfArguments != this->GetParameterCount())
	{
		throw NumberOfArgumentsException(location, this->GetParameterCount(), numberOfArguments);
	}

	vm->GetCallStack().Top().CallCodeBlock(location, vm, this->GetRootCodeBlock(), false);
}
