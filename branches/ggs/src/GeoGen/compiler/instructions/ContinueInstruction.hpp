#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class ContinueInstruction : public Instruction
			{
			private:
				int codeBlockCount;
			public:				
				ContinueInstruction(int codeBlockCount) 
				{
					this->codeBlockCount = codeBlockCount;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "Continue " << codeBlockCount << std::endl; }
			};
		}
	}
}