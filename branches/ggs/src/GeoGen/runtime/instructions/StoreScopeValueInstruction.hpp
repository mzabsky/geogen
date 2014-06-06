#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class StoreScopeValueInstruction : public Instruction
			{
			private:
				String variableName;
			public:				
				StoreScopeValueInstruction(CodeLocation location, String variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(IOStream& stream) const { stream << "StoreScopeValue " << variableName; }

				virtual String GetInstructionName() const { return "StoreScopeValue"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}