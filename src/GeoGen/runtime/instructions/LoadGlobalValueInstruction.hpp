#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class LoadGlobalValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:				
				LoadGlobalValueInstruction(CodeLocation location, int variableNameIndex) : Instruction(location)
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadGlobalValue " << variableNameIndex << std::endl; }
			};
		}
	}
}