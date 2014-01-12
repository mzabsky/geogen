#pragma once

#include <string>

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime 
	{
		namespace instructions
		{
			class CallGlobalInstruction : public Instruction
			{
			private:
				int functionNameIndex;
				int argumentCount;
			public:				
				CallGlobalInstruction(CodeLocation location, int functionNameIndex, int argumentCount) : Instruction(location)
				{
					this->functionNameIndex = functionNameIndex;
					this->argumentCount = argumentCount;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "CallGlobal " << functionNameIndex << " " << argumentCount << std::endl; }
			};
		}
	}
}