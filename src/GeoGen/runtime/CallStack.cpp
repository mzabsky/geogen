#include "CallStack.hpp"
#include "CallStackEntry.hpp"
#include "..\InternalErrorException.hpp"

using namespace geogen;
using namespace geogen::runtime;

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
		throw InternalErrorException("Can't get top of empty call stack.");
	}

	return *this->stack.back();
}

void CallStack::Pop()
{
	if (this->stack.size() < 1)
	{
		throw InternalErrorException("Can't get top of empty call stack.");
	}

	delete this->stack.back();

	this->stack.pop_back();
}

void CallStack::Push(CodeLocation location, FunctionDefinition const* functionDefinition)
{
	CallStackEntry* entry = new CallStackEntry(location, functionDefinition);

	this->stack.push_back(entry);
}

void CallStack::Serialize(std::iostream& stream) const
{
	stream << ">";
	for (const_reverse_iterator it = this->RBegin(); it != this->REnd(); it++)
	{
		stream << "\t";
		(*it)->Serialize(stream);
		stream << std::endl;
	}
}