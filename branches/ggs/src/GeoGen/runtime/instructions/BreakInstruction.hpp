#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class BreakInstruction : public Instruction
			{
			private:
				unsigned codeBlockCount;
			public:				
				BreakInstruction(CodeLocation location, unsigned codeBlockCount) : Instruction(location)
				{
					this->codeBlockCount = codeBlockCount;
				}

				virtual void Serialize(IOStream& stream) const { stream << "Break " << codeBlockCount; }

				virtual String GetInstructionName() const { return GG_STR("Break"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;

				virtual Instruction* Clone() const { return new BreakInstruction(this->GetLocation(), this->codeBlockCount); };
			};
		}
	}
}