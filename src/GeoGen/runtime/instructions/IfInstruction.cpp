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

#include <ostream>
#include <sstream>

#include "IfInstruction.hpp"
#include "../CodeBlockStackEntry.hpp"
#include "../VirtualMachine.hpp"
#include "../ManagedObject.hpp"
#include "../../corelib/BooleanTypeDefinition.hpp"
#include "../IncorrectTypeException.hpp"
#include "../../InternalErrorException.hpp"
#include "../TypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;
using namespace geogen::corelib;
using namespace geogen::runtime::instructions;

void IfInstruction::Serialize(IOStream& stream) const
{
	stream << GG_STR("If") << endl;
	stream << GG_STR("\ttrue") << endl;

	this->ifBranchCodeBlock.SerializeWithTabs(stream, 2);

	stream << GG_STR("\tfalse") << endl;
	this->elseBranchCodeBlock.SerializeWithTabs(stream, 2);
}

InstructionStepResult IfInstruction::Step(VirtualMachine* vm) const
{
	ManagedObject* conditionObject = vm->GetObjectStack().Top();				

	TypeDefinition const* boolTypeDefinition = vm->GetBooleanTypeDefinition();
	if (conditionObject->GetType() != boolTypeDefinition)
	{
		throw IncorrectTypeException(GGE2104_IncorrectConditionResultType, this->GetLocation(), boolTypeDefinition->GetName(), conditionObject->GetType()->GetName());
	}
	else if (conditionObject->IsStaticObject()){
		throw IncorrectTypeException(GGE2104_IncorrectConditionResultType, this->GetLocation(), boolTypeDefinition->GetName(), GG_STR("Type"));
	}

	if (dynamic_cast<BooleanObject*>(conditionObject)->GetValue())
	{
		vm->GetCallStack().Top().GetCodeBlockStack().Push(this->GetLocation(), &vm->GetMemoryManager(), this->GetIfBranchCodeBlock(), false);
	}
	else
	{
		vm->GetCallStack().Top().GetCodeBlockStack().Push(this->GetLocation(), &vm->GetMemoryManager(), this->GetElseBranchCodeBlock(), false);
	}

	vm->GetObjectStack().Pop(vm);
				
	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}

Instruction* IfInstruction::Clone() const
{
	IfInstruction* clone = new IfInstruction(this->GetLocation());
	clone->ifBranchCodeBlock = CodeBlock(this->ifBranchCodeBlock);
	clone->elseBranchCodeBlock = CodeBlock(this->elseBranchCodeBlock);
	return clone;
}
