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
				String variableName;
			public:
				StoreGlobalValueInstruction(CodeLocation location, String variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(IOStream& stream) const { stream << "StoreGlobalValue " << variableName; }

				virtual String GetInstructionName() const { return GG_STR("StoreGlobalValue"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;

				virtual Instruction* Clone() const { return new StoreGlobalValueInstruction(this->GetLocation(), this->variableName); };
			};
		}
	}
}