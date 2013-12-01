#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class StoreGlobalValueInstruction : public Instruction
			{
			private:
				int globalValueIndex;
			public:				
				StoreGlobalValueInstruction(int globalValueIndex) 
				{
					this->globalValueIndex = globalValueIndex;
				}
			};
		}
	}
}