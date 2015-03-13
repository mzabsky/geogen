/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include "CodeBlockStackEntry.hpp"
#include "instructions/Instruction.hpp"
#include "../InternalErrorException.hpp"

using namespace geogen;
using namespace geogen::runtime;

CodeBlockStackEntry::CodeBlockStackEntry(CodeLocation location, MemoryManager* memoryManager, CodeBlock const& codeBlock, bool isLooping)
	: location(location), codeBlock(&codeBlock), isLooping(isLooping), localVariableTable(memoryManager)
{
	this->codePointer = codeBlock.Begin();
}

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
		return CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED;
		//throw InternalErrorException("This code block has already finished (end of instruction list reached).");
	}

	InstructionStepResult instructionStepResult = (*this->codePointer)->Step(vm);

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
		throw InternalErrorException(GG_STR("Invalid item in CodeBlockStackEntryStepResultType."));
	}

	return codeBlockStepResult;
}

void CodeBlockStackEntry::Serialize(IOStream& stream) const
{
	if (this->GetCurrentInstruction() != NULL)
	{
		stream
			<< this->GetCurrentInstruction()->GetInstructionName()
			<< " on line " << this->GetCurrentInstruction()->GetLocation().GetLine()
			<< ", column " << this->GetCurrentInstruction()->GetLocation().GetLine();
	}
	else
	{
		stream << "<END>";
	}
}