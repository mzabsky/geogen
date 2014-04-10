#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class LoadConstBooleanInstruction : public Instruction
			{
			private:
				bool constBoolean;
			public:				
				LoadConstBooleanInstruction(CodeLocation location, bool constBoolean) : Instruction(location)
				{
					this->constBoolean = constBoolean;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadConstBoolean " << constBoolean; }

				virtual std::string GetInstructionName() const { return "LoadConstBoolean"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}