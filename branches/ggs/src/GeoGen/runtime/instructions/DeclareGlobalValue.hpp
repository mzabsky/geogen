#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class DeclareGlobalValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:
				DeclareGlobalValueInstruction(CodeLocation location, int variableNameIndex) : Instruction(location)
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "DeclareGlobalValue " << variableNameIndex << std::endl; }
			};
		}
	}
}