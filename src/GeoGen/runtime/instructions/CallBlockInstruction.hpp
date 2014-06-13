#pragma once

#include "Instruction.hpp"
#include "../CodeBlock.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			class CallBlockInstruction : public Instruction
			{
			private:
				CodeBlock codeBlock;
			public:
				CallBlockInstruction(CodeLocation location) : Instruction(location) {}

				inline CodeBlock& GetCodeBlock() { return this->codeBlock; };
				inline CodeBlock const& GetCodeBlock() const { return this->codeBlock; };

				virtual void Serialize(IOStream& stream) const;

				virtual String GetInstructionName() const { return GG_STR("CallCodeBlock"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}