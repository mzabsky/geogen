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

				virtual void Serialize(IOStream& stream) const { stream << "LoadNull"; }

				virtual String GetInstructionName() const { return GG_STR("LoadNull"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;

				virtual Instruction* Clone() const { return new LoadNullInstruction(this->GetLocation()); };
			};
		}
	}
}