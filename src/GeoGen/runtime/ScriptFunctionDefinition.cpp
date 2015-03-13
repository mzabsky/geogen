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

#include "ScriptFunctionDefinition.hpp"
#include "VirtualMachine.hpp"
#include "..\InternalErrorException.hpp"
#include "NumberOfArgumentsException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

void ScriptFunctionDefinition::Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const
{
	if (instance != NULL)
	{
		throw new InternalErrorException(GG_STR("Script functions cannot be instance methods."));
	}

	if (numberOfArguments != this->GetParameterCount())
	{
		throw NumberOfArgumentsException(location, this->GetParameterCount(), numberOfArguments);
	}

	vm->GetCallStack().Top().CallCodeBlock(location, vm, this->GetRootCodeBlock(), false);
}

void ScriptFunctionDefinition::Serialize(IOStream& stream) const
{
	stream << this->GetParameterCount() << GG_STR(" parameters on line ") << this->GetLocation().GetLine() << GG_STR(", column ") << this->GetLocation().GetColumn() << endl;
	this->rootCodeBlock.SerializeWithTabs(stream, 1);
}