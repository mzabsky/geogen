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

	std::cout << "\tCALL STACK STEP " << &top << std::endl;

	bool topIsLooping = top.IsLooping();
	CodeBlock const& topCodeBlock = top.GetCodeBlock();
	CodeBlockStackEntryStepResult codeBlockStepResult = top.Step(vm);

	if (codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED)
	{
		this->codeBlockStack.Pop();
	}

	std::cout << "\tCALL STACK STEP RESULT " << codeBlockStepResult << std::endl;

	// The top reference may now be pointing to an invalid address, because the entry was removed from the stack.

	/*else if (codeBlockStepResult.type == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_CONTINUE || codeBlockStepResult.type == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_BREAK)
	{ 
		if (codeBlockStepResult.codeBlockCount > this->codeBlockStack.size())
		{
			throw IntermediateCodeException("Break/continue code block count was greater than current code block nesting level.");
		}

		for (size_t i = 0; i < codeBlockStepResult.codeBlockCount; i++)
		{
			this->codeBlockStack.pop();
		}
	}*/

	// TODO: Sledovat lokalni promenne
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
	stream << this->functionDefinition->GetName() << std::endl;
}