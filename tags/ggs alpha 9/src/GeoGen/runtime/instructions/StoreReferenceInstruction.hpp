#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class StoreReferenceInstruction : public Instruction
			{
			private:
			public:
				StoreReferenceInstruction(CodeLocation location) : Instruction(location)
				{
				}

				virtual void Serialize(IOStream& stream) const { stream << "StoreReference"; }

				virtual String GetInstructionName() const { return GG_STR("StoreReference"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const { return INSTRUCTION_STEP_RESULT_TYPE_NORMAL; };
			};
		}
	}
}