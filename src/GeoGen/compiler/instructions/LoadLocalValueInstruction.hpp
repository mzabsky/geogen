#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class LoadLocalValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:				
				LoadLocalValueInstruction(int variableNameIndex) 
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadLocalValue " << variableNameIndex << std::endl; }
			};
		}
	}
}