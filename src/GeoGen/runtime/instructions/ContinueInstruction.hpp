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

				virtual void Serialize(std::iostream& stream) const { stream << "Continue " << codeBlockCount; }

				virtual std::string GetInstructionName() const { return "Continue"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}