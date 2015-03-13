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

#include "ContinueInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\IntermediateCodeException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult ContinueInstruction::Step(VirtualMachine* vm) const
{
	CodeBlockStack& codeBlockStack = vm->GetCallStack().Top().GetCodeBlockStack();

	codeBlockStack.CheckSize(this->codeBlockCount);

	for (unsigned i = 0; i < this->codeBlockCount; i++)
	{
		CodeBlockStackEntry& entry = codeBlockStack.Top();
		CodeBlock const& codeBlock = entry.GetCodeBlock();

		bool isLooping = entry.IsLooping();

		codeBlockStack.Pop();

		// Entry is now invalid pointer!

		if (isLooping)
		{
			if (i + 1 != this->codeBlockCount)
			{
				throw IntermediateCodeException(GG_STR("Continue attempted to cross through the topmost looping block."));
			}

			codeBlockStack.Push(this->GetLocation(), &vm->GetMemoryManager(), codeBlock, true);
		}		
	}

	return INSTRUCTION_STEP_RESULT_TYPE_CONTINUE;
}