#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class BreakInstruction : public Instruction
			{
			private:
				int codeBlockCount;
			public:				
				BreakInstruction(int codeBlockCount) 
				{
					this->codeBlockCount = codeBlockCount;
				}
			};
		}
	}
}