#pragma once

#include <string>

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class LoadConstStringInstruction : public Instruction
			{
			private:
				std::string constString;
			public:				
				LoadConstStringInstruction(CodeLocation location, std::string const& constString) : Instruction(location)
				{
					this->constString = constString;
				}

				virtual void Serialize(std::iostream& stream) const { stream << "LoadConstString " << constString << std::endl; }

				virtual std::string GetInstructionName() const { return "LoadConstString"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}