#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class LoadNullInstruction : public Instruction
			{
			private:
			public:
				LoadNullInstruction(CodeLocation location) : Instruction(location)
				{
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadNull"; }

				virtual std::string GetInstructionName() const { return "LoadNull"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}