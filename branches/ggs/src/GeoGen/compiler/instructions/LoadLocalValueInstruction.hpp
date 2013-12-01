#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class LoadLocalValueInstruction : public Instruction
			{
			private:
				int localValueIndex;
			public:				
				LoadLocalValueInstruction(int localValueIndex) 
				{
					this->localValueIndex = localValueIndex;
				}
			};
		}
	}
}