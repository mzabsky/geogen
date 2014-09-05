#pragma once

#include "Instruction.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class LoadScopeReferenceInstruction : public Instruction
			{
			private:
				String variableName;
			public:
				LoadScopeReferenceInstruction(CodeLocation location, String const& variableName) : Instruction(location)
				{
					this->variableName = variableName;
				}

				virtual void Serialize(IOStream& stream) const { stream << "LoadScopeReference " << variableName; }

				virtual String GetInstructionName() const { return GG_STR("LoadScopeReference"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;

				virtual Instruction* Clone() const { return new LoadScopeReferenceInstruction(this->GetLocation(), this->variableName); };
			};
		}
	}
}