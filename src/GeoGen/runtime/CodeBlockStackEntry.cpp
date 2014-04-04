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
};

CodeBlockStackEntry& CodeBlockStackEntry::operator=(CodeBlockStackEntry const& other)
{
	this->codeBlock = other.codeBlock;
	this->codePointer = other.codePointer;

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

	InstructionStepResult instructionStepResult = (*this->codePointer)->Step();
	this->codePointer = this->codePointer++;

	CodeBlockStackEntryStepResult codeBlockStepResult;
	codeBlockStepResult.codeBlockCount = instructionStepResult.codeBlockCount;
	switch (instructionStepResult.type)
	{
	case INSTRUCTION_STEP_RESULT_TYPE_NORMAL:
		if (this->codePointer != this->codeBlock->End())
		{
			codeBlockStepResult.type = CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_RUNNING;
		}
		else
		{
			codeBlockStepResult.type = CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED;
		}

		break;
	case INSTRUCTION_STEP_RESULT_TYPE_BREAK:
		codeBlockStepResult.type = CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_BREAK;
		break;
	case INSTRUCTION_STEP_RESULT_TYPE_CONTINUE:
		codeBlockStepResult.type = CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_CONTINUE;
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