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
				IfInstruction(CodeBlock& ifBranchCodeBlock, CodeBlock& elseBranchCodeBlock) 
				{
					this->ifBranchCodeBlock.MoveInstructionsFrom(ifBranchCodeBlock);
					this->elseBranchCodeBlock.MoveInstructionsFrom(elseBranchCodeBlock);
				}
			};
		}
	}
}