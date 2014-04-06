#pragma once

#include <string>

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class CallMemberInstruction : public Instruction
			{
			private:
				std::string functionName;
				int argumentCount;
			public:				
				CallMemberInstruction(CodeLocation location, std::string functionName, int argumentCount) : Instruction(location)
				{
					this->functionName = functionName;
					this->argumentCount = argumentCount;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "CallMember " << functionName << " " << argumentCount << std::endl; }

				virtual std::string GetInstructionName() const { return "CallMember"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}