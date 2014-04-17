#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class StoreGlobalValueInstruction : public Instruction
			{
			private:
				std::string variableName;
			public:
				StoreGlobalValueInstruction(CodeLocation location, std::string variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "StoreGlobalValue " << variableName; }

				virtual std::string GetInstructionName() const { return "StoreGlobalValue"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}