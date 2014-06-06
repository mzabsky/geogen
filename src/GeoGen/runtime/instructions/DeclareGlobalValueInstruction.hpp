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
				String variableName;
			public:
				DeclareGlobalValueInstruction(CodeLocation location, String variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(IOStream& stream) const { stream << "DeclareGlobalValue " << variableName; }

				virtual String GetInstructionName() const { return "DeclareGlobalValue"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}