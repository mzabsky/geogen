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

void CallStackEntry::CallCodeBlock(CodeBlock const& codeBlock, bool isLooping)
{
	CodeBlockStackEntry codeBlockStackEntry(codeBlock, isLooping);

	this->codeBlockStack.push(codeBlockStackEntry);
}

CallStackEntryStepResult CallStackEntry::Step(VirtualMachine* vm)
{
	if (this->codeBlockStack.empty())
	{
		throw InternalErrorException("The code block stack was empty (this call stack entry is already finished?)");
	}

	CodeBlockStackEntry& top = this->codeBlockStack.top();

	std::cout << "\tCALL STACK STEP " << &top << std::endl;

	bool topIsLooping = top.IsLooping();
	CodeBlock const& topCodeBlock = top.GetCodeBlock();
	CodeBlockStackEntryStepResult codeBlockStepResult = top.Step(vm);

	if (codeBlockStepResult == CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED)
	{
		this->codeBlockStack.pop();
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
		CodeBlockStackEntry newCodeBlockStackEntry(topCodeBlock, true);
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