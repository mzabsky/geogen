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

#include "CallStack.hpp"
#include "CallStackEntry.hpp"
#include "../InternalErrorException.hpp"
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