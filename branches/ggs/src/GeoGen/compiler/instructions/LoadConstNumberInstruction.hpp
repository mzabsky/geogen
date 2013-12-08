#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		namespace instructions
		{
			class LoadConstNumberInstruction : public Instruction
			{
			private:
				int constNumber;
			public:				
				LoadConstNumberInstruction(int constNumber) 
				{
					this->constNumber = constNumber;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadConstNumber " << constNumber << std::endl; }
			};
		}
	}
}