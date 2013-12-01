#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class StoreLocalValueInstruction : public Instruction
			{
			private:
				int localValueIndex;
			public:				
				StoreLocalValueInstruction(int localValueIndex) 
				{
					this->localValueIndex = localValueIndex;
				}
			};
		}
	}
}