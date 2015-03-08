#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class StoreMemberValueInstruction : public Instruction
			{
			private:
				String variableName;
			public:
				StoreMemberValueInstruction(CodeLocation location, String variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(IOStream& stream) const { stream << "StoreMemberValue " << variableName; }

				virtual String GetInstructionName() const { return GG_STR("StoreMemberValue"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;

				virtual Instruction* Clone() const { return new StoreMemberValueInstruction(this->GetLocation(), this->variableName); };
			};
		}
	}
}