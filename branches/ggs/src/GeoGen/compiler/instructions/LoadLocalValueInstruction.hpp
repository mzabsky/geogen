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

				virtual void Serialize(std::iostream& stream) const { stream << "LoadLocalValue " << localValueIndex << std::endl; }
			};
		}
	}
}