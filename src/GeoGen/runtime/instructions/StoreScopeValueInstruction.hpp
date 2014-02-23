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
				std::string variableName;
			public:				
				StoreScopeValueInstruction(CodeLocation location, std::string variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "StoreScopeValue " << variableName << std::endl; }
			};
		}
	}
}