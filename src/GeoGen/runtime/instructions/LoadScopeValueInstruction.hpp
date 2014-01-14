#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class LoadScopeValueInstruction : public Instruction
			{
			private:
				int variableNameIndex;
			public:				
				LoadScopeValueInstruction(CodeLocation location, int variableNameIndex) : Instruction(location)
				{
					this->variableNameIndex = variableNameIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadScopeValue " << variableNameIndex << std::endl; }
			};
		}
	}
}