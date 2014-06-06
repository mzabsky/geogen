#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class PopInstruction : public Instruction
			{
			private:
			public:				
				PopInstruction(CodeLocation location) : Instruction(location)
				{
				}

				virtual void Serialize(IOStream& stream) const { stream << "Pop"; }

				virtual String GetInstructionName() const { return "Pop"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}