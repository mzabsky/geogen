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
				String variableName;
			public:
				LoadMemberValueInstruction(CodeLocation location, String variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(IOStream& stream) const { stream << "LoadMemberValue " << variableName; }

				virtual String GetInstructionName() const { return "LoadMemberValue"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}