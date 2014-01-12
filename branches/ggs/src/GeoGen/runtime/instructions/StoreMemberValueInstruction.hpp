#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class StoreMemberValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:
				StoreMemberValueInstruction(CodeLocation location, int variableNameIndex) : Instruction(location)
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "StoreMemberValue " << variableNameIndex << std::endl; }
			};
		}
	}
}