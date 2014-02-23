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
				std::string variableName;
			public:				
				LoadScopeValueInstruction(CodeLocation location, std::string variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadScopeValue " << variableName << std::endl; }
			};
		}
	}
}