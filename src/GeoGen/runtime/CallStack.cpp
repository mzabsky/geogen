#include "CallStack.hpp"
#include "CallStackEntry.hpp"
#include "..\InternalErrorException.hpp"

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

	this->stack.pop_back();
}

void CallStack::Push(FunctionDefinition const* functionDefinition)
{
	CallStackEntry* entry = new CallStackEntry(functionDefinition);

	this->stack.push_back(entry);
}