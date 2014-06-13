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
				String variableName;
			public:
				DeclareLocalValueInstruction(CodeLocation location, String variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(IOStream& stream) const { stream << "DeclareLocalValue " << variableName; }

				virtual String GetInstructionName() const { return GG_STR("DeclareLocalValue"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}