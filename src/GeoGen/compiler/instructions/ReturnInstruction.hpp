#pragma once

#include <string>

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class ReturnInstruction : public Instruction
			{
			private:
			public:	
				virtual void Serialize(std::iostream& stream) const { stream << "Return" << std::endl; }
			};
		}
	}
}