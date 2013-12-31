#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace compiler
	{
		namespace instructions
		{
			class StoreMemberValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:
				StoreMemberValueInstruction(int variableNameIndex)
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "StoreMemberValue " << variableNameIndex << std::endl; }
			};
		}
	}
}