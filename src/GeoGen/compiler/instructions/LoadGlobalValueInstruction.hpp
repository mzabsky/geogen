#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class LoadGlobalValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:				
				LoadGlobalValueInstruction(int variableNameIndex) 
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadGlobalValue " << variableNameIndex << std::endl; }
			};
		}
	}
}