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
				int codeBlockCount;
			public:				
				ContinueInstruction(CodeLocation location, int codeBlockCount) : Instruction(location)
				{
					this->codeBlockCount = codeBlockCount;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "Continue " << codeBlockCount << std::endl; }

				virtual std::string GetInstructionName() const { return "Continue"; };
			};
		}
	}
}