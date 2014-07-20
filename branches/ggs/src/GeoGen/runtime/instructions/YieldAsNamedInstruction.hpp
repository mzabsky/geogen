#pragma once

#include "Instruction.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class YieldAsNamedInstruction : public Instruction
			{
			private:
				String functionName;
			public:				
				static const String MAP_NAME_MAIN;

				YieldAsNamedInstruction(CodeLocation location, String const& mapName) : Instruction(location)
				{
					this->functionName = mapName;
				}

				virtual void Serialize(IOStream& stream) const { stream << "YieldAsNamed " << functionName; }

				virtual String GetInstructionName() const { return GG_STR("YieldAsNamed"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}