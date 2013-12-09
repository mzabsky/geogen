#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class PopInstruction : public Instruction
			{
			private:
			public:				
				PopInstruction() 
				{
				}

				virtual void Serialize(std::iostream& stream) const { stream << "Pop" << std::endl; }
			};
		}
	}
}