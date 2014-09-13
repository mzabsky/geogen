#include "CallStackEntry.hpp"
#include "IntermediateCodeException.hpp"
#include "..\InternalErrorException.hpp"
#include "VirtualMachine.hpp"
#include "ScriptFunctionDefinition.hpp"
#include "CodeBlockStackEntry.hpp"

using namespace geogen;
using namespace geogen::runtime;

void CallStackEntry::CallCodeBlock(CodeLocation location, VirtualMachine* vm, CodeBlock const& codeBlock, bool isLooping)
{	
	this->codeBlockStack.Push(location, &vm->GetMemoryManager(), codeBlock, isLooping);
}

CallStackEntryStepResult CallStackEntry::Step(VirtualMachine* vm)
{
	if (this->codeBlockStack.IsEmpty())
	{
		throw InternalErrorException(GG_STR("The code block stack was empty (this call stack entry is already finished?)"));
	}

	int originalCodeBlockStackSize = this->codeBlockStack.Size();
	CodeBlockStackEntry& top = this->codeBlockStack.Top();


	bool topIsLooping = top.IsLooping();
	CodeBlock const& topCodeBlock = top.GetCodeBlock();
	CodeBlockStackEntryStepResult codeBlockStepResult = top.Step(vm);

	/*if (topIsLooping && (codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_CONTINUE || codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED))
	{
		this->CallCodeBlock(vm, topCodeBlock, true);
	}*/

	int codeBlockStackSizeDifference = this->codeBlockStack.Size() - originalCodeBlockStackSize;
	if (topIsLooping && codeBlockStackSizeDifference == 0 && codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED)
	{
		ScriptFunctionDefinition const* functionDefinition = dynamic_cast<ScriptFunctionDefinition const*>(this->GetFunctionDefinition());

		this->codeBlockStack.Pop();
		this->CallCodeBlock(functionDefinition->GetLocation(), vm, topCodeBlock, true);
	}
	/*else if(codeBlockStackSizeDifference < 0 && codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_CONTINUE){
		this->CallCodeBlock(vm, topCodeBlock, true);
	}*/
	
	while (!this->codeBlockStack.IsEmpty() && this->codeBlockStack.Top().GetCurrentInstruction() == NULL)
	{
		bool isCurrentCodeBlockStackEntryLooping = this->codeBlockStack.Top().IsLooping();
		CodeBlock const& currentCodeBlock = this->codeBlockStack.Top().GetCodeBlock();
		CodeLocation location = this->codeBlockStack.Top().GetLocation();

		this->codeBlockStack.Pop();

		if (isCurrentCodeBlockStackEntryLooping)
		{
			this->CallCodeBlock(location, vm, currentCodeBlock, true);
		}
	}
	/*
	if (codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED)
	{
		this->codeBlockStack.Pop();
	}

	if (topIsLooping && (codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_CONTINUE || codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED))
	{
		this->CallCodeBlock(vm, topCodeBlock, true);
	}*/	

	if (this->codeBlockStack.IsEmpty())
	{
		return CALL_STACK_ENTRY_STEP_RESULT_FINISHED;
	}	
	else
	{
		return CALL_STACK_ENTRY_STEP_RESULT_RUNNING;
	}
}

void CallStackEntry::Serialize(IOStream& stream) const
{
	stream << this->functionDefinition->GetName() << " on line " << this->GetCallLocation().GetLine() << ", column " << this->GetCallLocation().GetColumn();
}