#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class StoreLocalValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:				
				StoreLocalValueInstruction(int variableNameIndex) 
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "StoreLocalValue " << variableNameIndex << std::endl; }
			};
		}
	}
}