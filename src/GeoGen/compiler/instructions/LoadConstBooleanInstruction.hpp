#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class LoadConstBooleanInstruction : public Instruction
			{
			private:
				bool constBoolean;
			public:				
				LoadConstBooleanInstruction(bool constBoolean) 
				{
					this->constBoolean = constBoolean;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadConstBoolean " << constBoolean << std::endl; }
			};
		}
	}
}