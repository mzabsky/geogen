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

#include "../../String.hpp"
#include "../../Serializable.hpp"
#include "../../CodeLocation.hpp"

namespace geogen 
{
	namespace runtime
	{
		enum InstructionStepResult
		{
			INSTRUCTION_STEP_RESULT_TYPE_NORMAL,
			INSTRUCTION_STEP_RESULT_TYPE_CONTINUE,
			INSTRUCTION_STEP_RESULT_TYPE_BREAK
		};

		class VirtualMachine;

		namespace instructions
		{
			/// Base class for instruction implementations.
			class Instruction : public Serializable
			{
			private:
				CodeLocation location;
			protected:

				/// Constructor.
				/// @param location The location.
				Instruction(CodeLocation location) : location(location) {}
			public:				

				/// Gets the location.
				/// @return The location.
				inline CodeLocation GetLocation() const { return this->location; }

				/// Executes the instruction.
				/// @param [in,out] vm The virtual machine.
				/// @return Step result.
				virtual InstructionStepResult Step(VirtualMachine* vm) const 
				{ 
					InstructionStepResult result;
					result = INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
					//result.codeBlockCount = 0;
					return result; 
				};

				/// Gets the instruction name.
				/// @return The instruction name.
				virtual String GetInstructionName() const = 0;

				/// Makes a deep copy of this object.
				/// @return Deep copy of the instruction.
				virtual Instruction* Clone() const = 0;
			};
		}
	}
}