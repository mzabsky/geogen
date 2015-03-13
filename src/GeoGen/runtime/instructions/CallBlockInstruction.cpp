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

#include "CallBlockInstruction.hpp"
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

void CallBlockInstruction::Serialize(IOStream& stream) const
{
	stream << "CallBlock" << std::endl;

	StringStream substream;
	this->codeBlock.Serialize(substream);

	stream << substream.str();
}

InstructionStepResult CallBlockInstruction::Step(VirtualMachine* vm) const
{
	vm->GetCallStack().Top().GetCodeBlockStack().Push(this->GetLocation(), &vm->GetMemoryManager(), this->GetCodeBlock(), false);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}

Instruction* CallBlockInstruction::Clone() const
{
	CallBlockInstruction* clone = new CallBlockInstruction(this->GetLocation());
	clone->codeBlock = CodeBlock(this->codeBlock);
	return clone;
}
