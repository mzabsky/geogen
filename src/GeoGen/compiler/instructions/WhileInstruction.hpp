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
				WhileInstruction(CodeBlock& codeBlock) 
				{
					this->codeBlock.MoveInstructionsFrom(codeBlock);
				}

				virtual void Serialize(std::iostream& stream) const;
			};
		}
	}
}