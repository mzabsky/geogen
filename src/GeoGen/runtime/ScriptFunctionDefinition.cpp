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
		throw new InternalErrorException(GG_STR("Script functions cannot be instance methods."));
	}

	if (numberOfArguments != this->GetParameterCount())
	{
		throw NumberOfArgumentsException(location, this->GetParameterCount(), numberOfArguments);
	}

	vm->GetCallStack().Top().CallCodeBlock(location, vm, this->GetRootCodeBlock(), false);
}

void ScriptFunctionDefinition::Serialize(IOStream& stream) const
{
	stream << this->GetParameterCount() << GG_STR(" parameters on line ") << this->GetLocation().GetLine() << GG_STR(", column ") << this->GetLocation().GetColumn() << endl;
	this->rootCodeBlock.SerializeWithTabs(stream, 1);
}