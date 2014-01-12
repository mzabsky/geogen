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
				int codeBlockCount;
			public:				
				BreakInstruction(CodeLocation location, int codeBlockCount) : Instruction(location)
				{
					this->codeBlockCount = codeBlockCount;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "Break " << codeBlockCount << std::endl; }
			};
		}
	}
}