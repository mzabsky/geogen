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
				int functionNameIndex;
				int argumentCount;
			public:				
				CallMemberInstruction(int functionNameIndex, int argumentCount) 
				{
					this->functionNameIndex = functionNameIndex;
					this->argumentCount = argumentCount;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "CallMember " << functionNameIndex << " " << argumentCount << std::endl; }
			};
		}
	}
}