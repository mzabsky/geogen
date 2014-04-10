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

				virtual void Serialize(std::iostream& stream) const { stream << "Break " << codeBlockCount; }

				virtual std::string GetInstructionName() const { return "Break"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}