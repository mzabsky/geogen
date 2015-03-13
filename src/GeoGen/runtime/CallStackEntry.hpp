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

#include <vector>
#include <stack>
#include <map>

#include "../Serializable.hpp"
#include "CodeBlock.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "CodeBlockStack.hpp"

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;
		class VirtualMachine;
		class FunctionDefinition;

		/// Result of a call to CallStackEntry::Step.
		enum CallStackEntryStepResult
		{
			CALL_STACK_ENTRY_STEP_RESULT_RUNNING,
			CALL_STACK_ENTRY_STEP_RESULT_FINISHED
		};

		/// Call stack frame.
		class CallStackEntry : public Serializable
		{
		private:
			CodeLocation callLocation;
			FunctionDefinition const* functionDefinition;
			CodeBlockStack codeBlockStack;
			std::map<String, ManagedObject*> localVariableValues;

			CallStackEntry(CallStackEntry const& other) : callLocation(0, 0) {};
			CallStackEntry& operator=(CallStackEntry const& other) {};
		public:

			/// Constructor.
			/// @param location The code location.
			/// @param functionDefinition Definition of the called function.
			CallStackEntry(CodeLocation location, FunctionDefinition const* functionDefinition) : callLocation(location), functionDefinition(functionDefinition) {};

			/// Destructor.
			~CallStackEntry() {};

			/// Gets code location from which the function call originated.
			/// @return The call location.
			inline CodeLocation GetCallLocation() const { return this->callLocation; };

			/// Gets code block stack.
			/// @return The code block stack.
			inline CodeBlockStack& GetCodeBlockStack() { return this->codeBlockStack; };

			/// Gets code block stack.
			/// @return The code block stack.
			inline CodeBlockStack const& GetCodeBlockStack() const { return this->codeBlockStack; };

			/// Gets definition of the called function.
			/// @return The function definition.
			inline FunctionDefinition const* GetFunctionDefinition() const { return this->functionDefinition; }

			/// Adds a code block on top of the code block stack.
			/// @param location The call location.
			/// @param vm The virtual machine.
			/// @param codeBlock The called code block.
			/// @param isLooping True if the code block is to be called in a loop.
			void CallCodeBlock(CodeLocation location, VirtualMachine* vm, CodeBlock const& codeBlock, bool isLooping);

			/// Advances execution of the frame by one step.
			/// @param vm The virtual machine.
			/// @return step result.
			CallStackEntryStepResult Step(VirtualMachine* vm);

			virtual void Serialize(IOStream& stream) const;
		};
	}
}