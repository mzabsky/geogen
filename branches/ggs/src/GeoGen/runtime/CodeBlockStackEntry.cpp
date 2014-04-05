#include "CodeBlockStackEntry.hpp"
#include "../InternalErrorException.hpp"

using namespace geogen::runtime;

CodeBlockStackEntry::CodeBlockStackEntry(CodeBlock const& codeBlock, bool isLooping) : codeBlock(&codeBlock), isLooping(isLooping)
{
	this->codePointer = codeBlock.Begin();
}

CodeBlockStackEntry::CodeBlockStackEntry(CodeBlockStackEntry const& other)
{
	this->codeBlock = other.codeBlock;
	this->codePointer = other.codePointer;
	this->isLooping = other.isLooping;
};

CodeBlockStackEntry& CodeBlockStackEntry::operator=(CodeBlockStackEntry const& other)
{
	this->codeBlock = other.codeBlock;
	this->codePointer = other.codePointer;
	this->isLooping = other.isLooping;

	return *this;
};

instructions::Instruction const* CodeBlockStackEntry::GetCurrentInstruction() const
{
	if (this->codePointer != this->codeBlock->End())
	{
		return *this->codePointer;
	}
	else 
	{
		return NULL;
	}	
}

CodeBlockStackEntryStepResult CodeBlockStackEntry::Step(VirtualMachine* vm)
{
	if (this->codePointer == this->codeBlock->End())
	{
		throw InternalErrorException("This code block has already finished (end of instruction list reached).");
	}

	std::cout << "\t\tINSTRUCTION STEP " << (*this->codePointer)->ToString();

	InstructionStepResult instructionStepResult = (*this->codePointer)->Step(vm);

	std::cout << "\t\tINSTRUCTION STEP RESULT " << instructionStepResult << std::endl;

	// Note that "this" pointer may now be pointing to invalid addeess, because the entry was removed from the code block stack.
	// This should be indicated by appropriate instruction step result.

	CodeBlockStackEntryStepResult codeBlockStepResult;
	//codeBlockStepResult.codeBlockCount = instructionStepResult.codeBlockCount;
	switch (instructionStepResult)
	{
	case INSTRUCTION_STEP_RESULT_TYPE_NORMAL:
		this->codePointer++;
		if (this->codePointer != this->codeBlock->End())
		{			
			codeBlockStepResult = CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_RUNNING;
		}
		else
		{
			codeBlockStepResult = CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED;
		}

		break;
	case INSTRUCTION_STEP_RESULT_TYPE_BREAK:
		codeBlockStepResult = CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_BREAK;
		break;
	case INSTRUCTION_STEP_RESULT_TYPE_CONTINUE:
		codeBlockStepResult = CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_CONTINUE;
		break;
	default:
		throw InternalErrorException("Invalid item in CodeBlockStackEntryStepResultType.");
	}

	return codeBlockStepResult;
}

void CodeBlockStackEntry::Serialize(std::iostream& stream) const
{
	stream << this->GetCurrentInstruction()->GetInstructionName() << std::endl;
}