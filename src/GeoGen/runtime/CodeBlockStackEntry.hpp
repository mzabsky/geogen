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

#pragma once

#include <map>
#include <stack>

#include "CodeBlock.hpp"
#include "VariableTable.hpp"
#include "../Serializable.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;
		class VirtualMachine;

		/// Result of a call to CodeBlockStackEntry::Step.
		enum CodeBlockStackEntryStepResult
		{
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_RUNNING,
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED,
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_BREAK,
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_CONTINUE
		};

		/// A code block stack frame.
		class CodeBlockStackEntry : public Serializable
		{
		private:
			CodeBlock const* codeBlock;
			CodeBlock::const_iterator codePointer;
			bool isLooping;
			VariableTable localVariableTable;
			CodeLocation location;

			CodeBlockStackEntry(CodeBlockStackEntry const& other);
			CodeBlockStackEntry& operator=(CodeBlockStackEntry const& other);
		public:			

			/// Constructor.
			/// @param location The code block call location.
			/// @param memoryManager The memory manager.
			/// @param codeBlock The called code block.
			/// @param isLooping true if the code block is to be executed in a loop.
			CodeBlockStackEntry(CodeLocation location, MemoryManager* memoryManager, CodeBlock const& codeBlock, bool isLooping);

			/// Destructor.
			~CodeBlockStackEntry() {};

			/// Query if this code block is being executed in a loop.
			/// @return true if the code block is looping.
			inline bool IsLooping() const { return this->isLooping; };

			/// Gets the location from which the code block was called.
			/// @return The call location.
			inline CodeLocation GetLocation() const { return this->location; };

			/// Gets code block being called.
			/// @return The code block.
			inline CodeBlock const& GetCodeBlock() const { return *this->codeBlock; };

			/// Gets local variable table.
			/// @return The local variable table.
			inline VariableTable& GetLocalVariableTable() { return this->localVariableTable; };

			/// Gets current instruction.
			/// @return The current instruction.
			const instructions::Instruction* GetCurrentInstruction() const;

			/// Advances execution by a single step.
			/// @param vm The virtual machine.
			/// @return A step result.
			CodeBlockStackEntryStepResult Step(VirtualMachine* vm);

			virtual void Serialize(IOStream& stream) const;
		};
	}
}