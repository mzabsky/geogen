#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class YieldAsMainInstruction : public Instruction
			{
			private:
			public:				
				YieldAsMainInstruction(CodeLocation location) : Instruction(location)
				{
				}

				virtual void Serialize(std::iostream& stream) const { stream << "YieldAsMain" << std::endl; }

				virtual std::string GetInstructionName() const { return "YieldAsMain"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}