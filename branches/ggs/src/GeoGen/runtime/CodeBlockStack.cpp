#include "CodeBlockStack.hpp"
#include "CodeBlockStackEntry.hpp"
#include "..\InternalErrorException.hpp"

using namespace geogen::runtime;

CodeBlockStack::~CodeBlockStack()
{
	for (const_iterator it = this->Begin(); it < this->End(); it++)
	{
		delete (*it);
	}
}

CodeBlockStackEntry& CodeBlockStack::Top()
{
	if (this->stack.size() < 1)
	{
		throw InternalErrorException("Can't get top of empty code block stack.");
	}

	return *this->stack.back();
}

void CodeBlockStack::Pop()
{
	if (this->stack.size() < 1)
	{
		throw InternalErrorException("Can't get top of empty code block stack.");
	}

	this->stack.pop_back();
}

void CodeBlockStack::Push(MemoryManager* memoryManager, CodeBlock const& codeBlock, bool isLooping)
{
	CodeBlockStackEntry* entry = new CodeBlockStackEntry(memoryManager, codeBlock, isLooping);

	this->stack.push_back(entry);
}