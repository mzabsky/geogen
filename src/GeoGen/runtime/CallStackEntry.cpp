#include "CallStackEntry.hpp"
#include "IntermediateCodeException.hpp"
#include "..\InternalErrorException.hpp"
#include "VirtualMachine.hpp"

using namespace geogen::runtime;

void CallStackEntry::CallCodeBlock(VirtualMachine* vm, CodeBlock const& codeBlock, bool isLooping)
{
	this->codeBlockStack.Push(&vm->GetMemoryManager(), codeBlock, isLooping);
}

CallStackEntryStepResult CallStackEntry::Step(VirtualMachine* vm)
{
	if (this->codeBlockStack.IsEmpty())
	{
		throw InternalErrorException("The code block stack was empty (this call stack entry is already finished?)");
	}

	CodeBlockStackEntry& top = this->codeBlockStack.Top();

	bool topIsLooping = top.IsLooping();
	CodeBlock const& topCodeBlock = top.GetCodeBlock();
	CodeBlockStackEntryStepResult codeBlockStepResult = top.Step(vm);

	if (codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED)
	{
		this->codeBlockStack.Pop();
	}

	if (topIsLooping && (codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_CONTINUE || codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED))
	{
		this->CallCodeBlock(vm, topCodeBlock, true);
	}	

	if (this->codeBlockStack.IsEmpty())
	{
		return CALL_STACK_ENTRY_STEP_RESULT_FINISHED;
	}	
	else
	{
		return CALL_STACK_ENTRY_STEP_RESULT_RUNNING;
	}
}

void CallStackEntry::Serialize(std::iostream& stream) const
{
	stream << this->functionDefinition->GetName() << " on line " << this->GetCallLocation().GetLine() << ", column " << this->GetCallLocation().GetColumn();
}