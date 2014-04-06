#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class DeclareLocalValueInstruction : public Instruction
			{
			private:
				std::string variableName;
			public:
				DeclareLocalValueInstruction(CodeLocation location, std::string variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "DeclareLocalValue " << variableName << std::endl; }

				virtual std::string GetInstructionName() const { return "DeclareLocalValue"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}