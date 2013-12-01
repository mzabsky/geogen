#pragma once

#include <string>

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class CallGlobalInstruction : public Instruction
			{
			private:
				std::string functionName;
				int argumentCount;
			public:				
				CallGlobalInstruction(std::string const& functionName, int argumentCount) 
				{
					this->functionName = functionName;
					this->argumentCount = argumentCount;
				}
			};
		}
	}
}