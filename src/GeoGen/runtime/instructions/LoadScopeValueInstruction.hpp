#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class LoadScopeValueInstruction : public Instruction
			{
			private:
				String variableName;
			public:				
				LoadScopeValueInstruction(CodeLocation location, String variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(IOStream& stream) const { stream << "LoadScopeValue " << variableName; }

				virtual String GetInstructionName() const { return "LoadScopeValue"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}