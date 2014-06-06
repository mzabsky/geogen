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
			class Instruction : public Serializable
			{
			private:
				CodeLocation location;
			protected:
				Instruction(CodeLocation location) : location(location) {}
			public:				
				inline CodeLocation GetLocation() const { return this->location; }

				virtual InstructionStepResult Step(VirtualMachine* vm) const 
				{ 
					InstructionStepResult result;
					result = INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
					//result.codeBlockCount = 0;
					return result; 
				};

				virtual String GetInstructionName() const = 0;
			};
		}
	}
}