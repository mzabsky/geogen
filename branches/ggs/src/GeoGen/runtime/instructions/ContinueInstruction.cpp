#include "ContinueInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\IntermediateCodeException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult ContinueInstruction::Step(VirtualMachine* vm) const
{
	CodeBlockStack& codeBlockStack = vm->GetCallStack().Top().GetCodeBlockStack();

	codeBlockStack.CheckSize(this->codeBlockCount);

	for (unsigned i = 0; i < this->codeBlockCount; i++)
	{
		CodeBlockStackEntry& entry = codeBlockStack.Top();
		CodeBlock const& codeBlock = entry.GetCodeBlock();

		bool isLooping = entry.IsLooping();

		codeBlockStack.Pop();

		// Entry is now invalid pointer!

		if (isLooping)
		{
			if (i + 1 != this->codeBlockCount)
			{
				throw IntermediateCodeException("Continue attempted to cross through the topmost looping block.");
			}

			codeBlockStack.Push(this->GetLocation(), &vm->GetMemoryManager(), codeBlock, true);
		}		
	}

	return INSTRUCTION_STEP_RESULT_TYPE_CONTINUE;
}