#pragma once

#include <string>
#include <ostream>

#include "..\..\Serializable.hpp"
#include "..\..\CodeLocation.hpp"

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

		/*struct InstructionStepResult
		{
			InstructionStepResultType type;
			int codeBlockCount;
		};*/

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

				virtual InstructionStepResult Step() const 
				{ 
					InstructionStepResult result;
					result = INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
					//result.codeBlockCount = 0;
					return result; 
				};

				virtual std::string GetInstructionName() const = 0;
			};
		}
	}
}