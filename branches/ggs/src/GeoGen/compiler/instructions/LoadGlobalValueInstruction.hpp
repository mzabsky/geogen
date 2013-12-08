#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class LoadGlobalValueInstruction : public Instruction
			{
			private:
				int globalValueIndex;
			public:				
				LoadGlobalValueInstruction(int globalValueIndex) 
				{
					this->globalValueIndex = globalValueIndex;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadGlobalValue " << globalValueIndex << std::endl; }
			};
		}
	}
}