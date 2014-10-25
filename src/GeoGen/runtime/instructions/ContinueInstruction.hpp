#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class ContinueInstruction : public Instruction
			{
			private:
				unsigned codeBlockCount;
			public:				
				ContinueInstruction(CodeLocation location, unsigned codeBlockCount) : Instruction(location)
				{
					this->codeBlockCount = codeBlockCount;
				}

				virtual void Serialize(IOStream& stream) const { stream << "Continue " << codeBlockCount; }

				virtual String GetInstructionName() const { return GG_STR("Continue"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;

				virtual Instruction* Clone() const { return new ContinueInstruction(this->GetLocation(), this->codeBlockCount); };
			};
		}
	}
}