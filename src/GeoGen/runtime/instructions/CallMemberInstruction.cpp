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

#include "CallMemberInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\UndefinedSymbolAccessException.hpp"
#include "..\ManagedObject.hpp"
#include "..\NullReferenceException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult CallMemberInstruction::Step(VirtualMachine* vm) const
{
	vm->GetObjectStack().CheckSize(1);
	ManagedObject* instance = vm->GetObjectStack().Top();
	instance->AddRef();
	vm->GetObjectStack().Pop(vm);

	FunctionDefinition const* functionDefinition;
	
	if (instance == vm->GetNull())
	{
		throw NullReferenceException(this->GetLocation());
	}

	if (instance->IsStaticObject())
	{
		functionDefinition = instance->GetType()->GetStaticFunctionDefinitions().GetItem(this->functionName);
	}
	else
	{
		functionDefinition = instance->GetType()->GetFunctionDefinitions().GetItem(this->functionName);
	}
	
	if (functionDefinition == NULL)
	{
		throw UndefinedSymbolAccessException(GGE2201_UndefinedFunction, this->GetLocation(), this->functionName);
	}

	vm->CallFunction(this->GetLocation(), functionDefinition, instance, this->argumentCount);

	instance->RemoveRef(vm->GetMemoryManager());

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}