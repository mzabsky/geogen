#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class LoadMemberValueInstruction : public Instruction
			{
			private:
				std::string variableName;
			public:
				LoadMemberValueInstruction(CodeLocation location, std::string variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadMemberValue " << variableName; }

				virtual std::string GetInstructionName() const { return "LoadMemberValue"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}