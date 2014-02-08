#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class DeclareLocalValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:
				DeclareLocalValueInstruction(CodeLocation location, int variableNameIndex) : Instruction(location)
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "DeclareLocalValue " << variableNameIndex << std::endl; }
			};
		}
	}
}