#include "CallStack.hpp"
#include "CallStackEntry.hpp"
#include "..\InternalErrorException.hpp"
#include "StackOverflowException.hpp"

using namespace geogen;
using namespace geogen::runtime;

const unsigned CallStack::SIZE_LIMIT = 1000;

CallStack::~CallStack()
{
	for (const_iterator it = this->Begin(); it < this->End(); it++)
	{
		delete (*it);
	}
}

CallStackEntry& CallStack::Top()
{
	if (this->stack.size() < 1)
	{
		throw InternalErrorException(GG_STR("Can't get top of empty call stack."));
	}

	return *this->stack.back();
}

CallStackEntry const& CallStack::Top() const
{
	if (this->stack.size() < 1)
	{
		throw InternalErrorException(GG_STR("Can't get top of empty call stack."));
	}

	return *this->stack.back();
}

void CallStack::Pop()
{
	if (this->stack.size() < 1)
	{
		throw InternalErrorException(GG_STR("Can't get top of empty call stack."));
	}

	delete this->stack.back();

	this->stack.pop_back();
}

void CallStack::Push(CodeLocation location, FunctionDefinition const* functionDefinition)
{
	CallStackEntry* entry = new CallStackEntry(location, functionDefinition);

	if (SIZE_LIMIT == this->stack.size())
	{
		throw StackOverflowException(GGE2501_CallStackOverflow, location, STACK_TYPE_CALL);
	}

	this->stack.push_back(entry);
}

void CallStack::Serialize(IOStream& stream) const
{
	stream << ">";
	for (const_reverse_iterator it = this->RBegin(); it != this->REnd(); it++)
	{
		stream << "\t";
		(*it)->Serialize(stream);
		stream << std::endl;
	}
}