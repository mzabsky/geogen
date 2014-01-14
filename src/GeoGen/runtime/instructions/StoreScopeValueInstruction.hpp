#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class StoreScopeValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:				
				StoreScopeValueInstruction(CodeLocation location, int variableNameIndex) : Instruction(location)
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "StoreScopeValue " << variableNameIndex << std::endl; }
			};
		}
	}
}