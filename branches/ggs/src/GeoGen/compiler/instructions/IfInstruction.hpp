#pragma once

#include "Instruction.hpp"
#include "../CodeBlock.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class IfInstruction : public Instruction
			{
			private:
				CodeBlock ifBranchCodeBlock;
				CodeBlock elseBranchCodeBlock;
			public:				
				IfInstruction(CodeBlock const& ifBranchCodeBlock, CodeBlock const& elseBranchCodeBlock) 
				{
					this->ifBranchCodeBlock = ifBranchCodeBlock;
					this->elseBranchCodeBlock = elseBranchCodeBlock;
				}
			};
		}
	}
}