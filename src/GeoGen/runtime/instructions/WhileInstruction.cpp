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

#include "WhileInstruction.hpp"
#include "../CodeBlockStackEntry.hpp"
#include "../VirtualMachine.hpp"

using namespace geogen;
using namespace runtime;
using namespace instructions;
		
void WhileInstruction::Serialize(IOStream& stream) const
{
	stream << "While" << std::endl;
	this->codeBlock.SerializeWithTabs(stream, 1);
}

InstructionStepResult WhileInstruction::Step(VirtualMachine* vm) const
{
	vm->GetCallStack().Top().GetCodeBlockStack().Push(this->GetLocation(), &vm->GetMemoryManager(), this->GetCodeBlock(), true);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}

Instruction* WhileInstruction::Clone() const
{
	WhileInstruction* clone = new WhileInstruction(this->GetLocation());
	clone->codeBlock = CodeBlock(this->codeBlock);
	return clone;
}
