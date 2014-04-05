#include "ScriptFunctionDefinition.hpp"
#include "VirtualMachine.hpp"

using namespace geogen::runtime;
using namespace std;

void ScriptFunctionDefinition::Call(VirtualMachine* vm) const
{
	vm->GetCallStack().top().CallCodeBlock(this->GetRootCodeBlock(), false);
}
