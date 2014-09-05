#include <ostream>
#include <sstream>

#include "CallBlockInstruction.hpp"
#include "../CodeBlockStackEntry.hpp"
#include "../VirtualMachine.hpp"
#include "../ManagedObject.hpp"
#include "../../corelib/BooleanTypeDefinition.hpp"
#include "../IncorrectTypeException.hpp"
#include "../../InternalErrorException.hpp"
#include "../TypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;
using namespace geogen::corelib;
using namespace geogen::runtime::instructions;

void CallBlockInstruction::Serialize(IOStream& stream) const
{
	stream << "CallBlock" << std::endl;

	StringStream substream;
	this->codeBlock.Serialize(substream);

	stream << substream.str();
}

InstructionStepResult CallBlockInstruction::Step(VirtualMachine* vm) const
{
	vm->GetCallStack().Top().GetCodeBlockStack().Push(this->GetLocation(), &vm->GetMemoryManager(), this->GetCodeBlock(), false);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}

Instruction* CallBlockInstruction::Clone() const
{
	CallBlockInstruction* clone = new CallBlockInstruction(this->GetLocation());
	clone->codeBlock = CodeBlock(this->codeBlock);
	return clone;
}
