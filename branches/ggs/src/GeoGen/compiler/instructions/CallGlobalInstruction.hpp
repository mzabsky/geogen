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
				int functionNameIndex;
				int argumentCount;
			public:				
				CallGlobalInstruction(int functionNameIndex, int argumentCount) 
				{
					this->functionNameIndex = functionNameIndex;
					this->argumentCount = argumentCount;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "CallGlobal " << functionNameIndex << " " << argumentCount << std::endl; }
			};
		}
	}
}