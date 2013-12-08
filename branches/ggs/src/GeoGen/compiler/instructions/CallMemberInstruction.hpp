#pragma once

#include <string>

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class CallMemberInstruction : public Instruction
			{
			private:
				std::string functionName;
				int argumentCount;
			public:				
				CallMemberInstruction(std::string const& functionName, int argumentCount) 
				{
					this->functionName = functionName;
					this->argumentCount = argumentCount;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "CallMember " << functionName << " " << argumentCount << std::endl; }
			};
		}
	}
}