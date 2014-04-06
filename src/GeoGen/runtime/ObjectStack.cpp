#include "ObjectStack.hpp"
#include "DynamicObject.hpp"
#include "..\InternalErrorException.hpp"

using namespace geogen::runtime;

/*CodeBlockStack::~CodeBlockStack()
{
	for (const_iterator it = this->Begin(); it < this->End(); it++)
	{
		delete (*it);
	}
}*/

DynamicObject* ObjectStack::Top()
{
	if (this->stack.size() < 1)
	{
		throw InternalErrorException("Can't get top of empty code block stack.");
	}

	return this->stack.back();
}

void ObjectStack::Pop()
{
	if (this->stack.size() < 1)
	{
		throw InternalErrorException("Can't get top of empty code block stack.");
	}

	this->stack.pop_back();
}

void ObjectStack::Push(DynamicObject* object)
{	
	this->stack.push_back(object);
}

void ObjectStack::CheckSize(unsigned requiredSize)
{
	if (this->stack.size() < requiredSize)
	{
		throw InternalErrorException("Object stack has fewer items than required for current operation.");
	}
}