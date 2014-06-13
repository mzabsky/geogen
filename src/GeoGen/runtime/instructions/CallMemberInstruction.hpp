#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class CallMemberInstruction : public Instruction
			{
			private:
				String functionName;
				int argumentCount;
			public:				
				CallMemberInstruction(CodeLocation location, String functionName, int argumentCount) : Instruction(location)
				{
					this->functionName = functionName;
					this->argumentCount = argumentCount;
				}

				virtual void Serialize(IOStream& stream) const { stream << "CallMember " << functionName << " " << argumentCount; }

				virtual String GetInstructionName() const { return GG_STR("CallMember"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}