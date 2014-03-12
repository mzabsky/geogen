#include "CallStackEntry.hpp"
#include "IntermediateCodeException.hpp"
#include "..\InternalErrorException.hpp"

using namespace geogen::runtime;

CallStackEntry::CallStackEntry(CallStackEntry const& other) 
{
	this->codeBlockStack = other.codeBlockStack;
	this->functionDefinition = other.functionDefinition;
};

CallStackEntry& CallStackEntry::operator=(CallStackEntry const& other) 
{
	
	this->codeBlockStack = other.codeBlockStack;
	this->functionDefinition = other.functionDefinition;

	return *this;
};

CallStackEntryStepResult CallStackEntry::Step(VirtualMachine* vm)
{
	if (this->codeBlockStack.empty())
	{
		throw InternalErrorException("The code block stack was empty (this call stack entry is already finished?)");
	}

	CodeBlockStackEntry top = this->codeBlockStack.top();
	CodeBlockStackEntryStepResult codeBlockStepResult = top.Step(vm);

	if (codeBlockStepResult.type == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED)
	{
		this->codeBlockStack.pop();
	}
	else if (codeBlockStepResult.type == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_CONTINUE || codeBlockStepResult.type == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_BREAK)
	{ 
		if (codeBlockStepResult.codeBlockCount > this->codeBlockStack.size())
		{
			throw IntermediateCodeException("Break/continue code block count was greater than current code block nesting level.");
		}

		for (size_t i = 0; i < codeBlockStepResult.codeBlockCount; i++)
		{
			this->codeBlockStack.pop();
		}
	}

	// TODO: Sledovat lokalni promenne
	if (top.IsLooping() && codeBlockStepResult.type != CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_BREAK)
	{
		CodeBlockStackEntry newCodeBlockStackEntry(top.GetCodeBlock(), true);
		this->codeBlockStack.push(newCodeBlockStackEntry);
	}	

	if (this->codeBlockStack.empty())
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