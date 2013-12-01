#pragma once

#include "Instruction.hpp"
#include "../CodeBlock.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class WhileInstruction : public Instruction
			{
			private:
				CodeBlock codeBlock;
			public:				
				WhileInstruction(CodeBlock const& codeBlock) 
				{
					this->codeBlock = codeBlock;
				}
			};
		}
	}
}