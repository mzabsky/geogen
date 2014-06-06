#pragma once

#include "../../Number.hpp"
#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class LoadConstNumberInstruction : public Instruction
			{
			private:
				Number constNumber;
			public:				
				LoadConstNumberInstruction(CodeLocation location, Number constNumber) : Instruction(location)
				{
					this->constNumber = constNumber;
				}

				virtual void Serialize(IOStream& stream) const { stream << "LoadConstNumber " << constNumber; }

				virtual String GetInstructionName() const { return "LoadConstNumber"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}