#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class StoreGlobalValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:				
				StoreGlobalValueInstruction(CodeLocation location, int variableNameIndex) : Instruction(location)
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "StoreGlobalValue " << variableNameIndex << std::endl; }
			};
		}
	}
}