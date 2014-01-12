#pragma once

#include <string>

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class YieldAsNamedInstruction : public Instruction
			{
			private:
				std::string functionName;
			public:				
				YieldAsNamedInstruction(CodeLocation location, std::string const& mapName) : Instruction(location)
				{
					this->functionName = mapName;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "YieldAsNamed " << functionName << std::endl; }
			};
		}
	}
}