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

#include "StoreMemberValueInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\NullReferenceException.hpp"
#include "..\ManagedObject.hpp"
#include "..\UndefinedSymbolAccessException.hpp"
#include "..\ReadOnlyWriteException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult StoreMemberValueInstruction::Step(VirtualMachine* vm) const
{
	ManagedObject* instance = vm->GetObjectStack().Top();	

	if (instance == vm->GetNull())
	{
		throw NullReferenceException(this->GetLocation());
	}

	ManagedObject* value = vm->GetObjectStack().Top();

	VariableTableItem* variableTableItem = instance->GetMemberVariableTable().GetVariable(this->variableName);
	if (variableTableItem == NULL)
	{
		throw UndefinedSymbolAccessException(GGE2202_UndefinedVariable, this->GetLocation(), this->variableName);
	}

	if (!variableTableItem->SetValue(vm, value))
	{
		throw ReadOnlyWriteException(this->GetLocation(), this->variableName);
	}

	vm->GetObjectStack().Pop(vm);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}