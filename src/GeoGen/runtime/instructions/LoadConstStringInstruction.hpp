#pragma once

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
				String constString;
			public:				
				LoadConstStringInstruction(CodeLocation location, String const& constString) : Instruction(location)
				{
					this->constString = constString;
				}

				virtual void Serialize(IOStream& stream) const { stream << "LoadConstString " << constString; }

				virtual String GetInstructionName() const { return GG_STR("LoadConstString"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;

				virtual Instruction* Clone() const { return new LoadConstStringInstruction(this->GetLocation(), this->constString); };
			};
		}
	}
}