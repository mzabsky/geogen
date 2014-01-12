#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace compiler
	{
		namespace instructions
		{
			class LoadMemberValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:
				LoadMemberValueInstruction(CodeLocation location, int variableNameIndex) : Instruction(location)
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadMemberValue " << variableNameIndex << std::endl; }
			};
		}
	}
}