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
				YieldAsNamedInstruction(std::string const& mapName) 
				{
					this->functionName = mapName;
				}
			};
		}
	}
}