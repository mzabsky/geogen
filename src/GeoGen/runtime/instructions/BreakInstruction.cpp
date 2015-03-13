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

#include "BreakInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult BreakInstruction::Step(VirtualMachine* vm) const
{
	CodeBlockStack& codeBlockStack = vm->GetCallStack().Top().GetCodeBlockStack();

	codeBlockStack.CheckSize(this->codeBlockCount);

	for (unsigned i = 0; i < this->codeBlockCount; i++)
	{
		codeBlockStack.Pop();
	}

	return INSTRUCTION_STEP_RESULT_TYPE_BREAK;
}