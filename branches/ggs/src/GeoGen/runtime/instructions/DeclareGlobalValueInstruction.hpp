#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class DeclareGlobalValueInstruction : public Instruction
			{
			private:
				std::string variableName;
			public:
				DeclareGlobalValueInstruction(CodeLocation location, std::string variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "DeclareGlobalValue " << variableName << std::endl; }

				virtual std::string GetInstructionName() const { return "DeclareGlobalValue"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}