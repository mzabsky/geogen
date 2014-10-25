#include <ostream>
#include <sstream>

#include "WhileInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"

using namespace geogen;
using namespace runtime;
using namespace instructions;
		
void WhileInstruction::Serialize(IOStream& stream) const
{
	stream << "While" << std::endl;
	this->codeBlock.SerializeWithTabs(stream, 1);
}

InstructionStepResult WhileInstruction::Step(VirtualMachine* vm) const
{
	vm->GetCallStack().Top().GetCodeBlockStack().Push(this->GetLocation(), &vm->GetMemoryManager(), this->GetCodeBlock(), true);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}

Instruction* WhileInstruction::Clone() const
{
	WhileInstruction* clone = new WhileInstruction(this->GetLocation());
	clone->codeBlock = CodeBlock(this->codeBlock);
	return clone;
}
