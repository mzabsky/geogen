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
				std::string variableName;
			public:
				StoreMemberValueInstruction(CodeLocation location, std::string variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "StoreMemberValue " << variableName << std::endl; }
			};
		}
	}
}