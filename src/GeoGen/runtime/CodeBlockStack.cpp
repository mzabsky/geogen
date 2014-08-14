#include "CodeBlockStack.hpp"
#include "CodeBlockStackEntry.hpp"
#include "..\InternalErrorException.hpp"
#include "StackOverflowException.hpp"

using namespace geogen;
using namespace geogen::runtime;

const unsigned CodeBlockStack::SIZE_LIMIT = 1000;

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
		throw InternalErrorException(GG_STR("Can't get top of empty code block stack."));
	}

	return *this->stack.back();
}

CodeBlockStackEntry const& CodeBlockStack::Top() const
{
	if (this->stack.size() < 1)
	{
		throw InternalErrorException(GG_STR("Can't get top of empty code block stack."));
	}

	return *this->stack.back();
}

void CodeBlockStack::Pop()
{
	if (this->stack.size() < 1)
	{
		throw InternalErrorException(GG_STR("Can't get top of empty code block stack."));
	}

	delete this->stack.back();

	this->stack.pop_back();
}

void CodeBlockStack::Push(CodeLocation location, MemoryManager* memoryManager, CodeBlock const& codeBlock, bool isLooping)
{
	if (SIZE_LIMIT == this->stack.size())
	{
		throw StackOverflowException(GGE2502_CodeBlockStackOverflow, location, STACK_TYPE_CODE_BLOCK);
	}

	// Empty code block can be ignored
	if (std::distance(codeBlock.Begin(), codeBlock.End()) > 0)
	{
		CodeBlockStackEntry* entry = new CodeBlockStackEntry(location, memoryManager, codeBlock, isLooping);

		this->stack.push_back(entry);
	}
}

void CodeBlockStack::CheckSize(unsigned requiredSize) const
{
	if (this->stack.size() < requiredSize)
	{
		throw InternalErrorException(GG_STR("Code block stack has fewer items than required for current operation."));
	}
}

void CodeBlockStack::Serialize(IOStream& stream) const
{
	stream << ">";
	for (const_reverse_iterator it = this->RBegin(); it != this->REnd(); it++)
	{
		stream << "\t";
		(*it)->Serialize(stream);
		stream << std::endl;
	}
}