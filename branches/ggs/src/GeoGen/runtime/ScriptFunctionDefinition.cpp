#include "ScriptFunctionDefinition.hpp"
#include "VirtualMachine.hpp"

using namespace geogen::runtime;
using namespace std;

void ScriptFunctionDefinition::Call(VirtualMachine* vm) const
{
	vm->GetCallStack().Top().CallCodeBlock(vm, this->GetRootCodeBlock(), false);
}
