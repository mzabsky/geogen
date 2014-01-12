#pragma once

#include "Instruction.hpp"
#include "../CodeBlock.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class IfInstruction : public Instruction
			{
			private:
				CodeBlock ifBranchCodeBlock;
				CodeBlock elseBranchCodeBlock;
			public:				
				IfInstruction(CodeLocation location) : Instruction(location) {}

				inline CodeBlock& GetIfBranchCodeBlock() { return this->ifBranchCodeBlock; };
				inline CodeBlock const& GetIfBranchCodeBlock() const { return this->ifBranchCodeBlock; };
				
				inline CodeBlock& GetElseBranchCodeBlock() { return this->elseBranchCodeBlock; };
				inline CodeBlock const& GetElseBranchCodeBlock() const { return this->elseBranchCodeBlock; };

				virtual void Serialize(std::iostream& stream) const;
			};
		}
	}
}