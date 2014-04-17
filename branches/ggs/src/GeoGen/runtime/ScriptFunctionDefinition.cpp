#include "ScriptFunctionDefinition.hpp"
#include "VirtualMachine.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

void ScriptFunctionDefinition::Call(CodeLocation location, VirtualMachine* vm, unsigned numberOfArguments) const
{
	// TODO: Validace argumentu

	vm->GetCallStack().Top().CallCodeBlock(location, vm, this->GetRootCodeBlock(), false);
}
