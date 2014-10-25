#pragma once

#include "Instruction.hpp"
#include "../CodeBlock.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			class WhileInstruction : public Instruction
			{
			private:
				CodeBlock codeBlock;
			public:
				WhileInstruction(CodeLocation location) : Instruction(location) {};

				virtual void Serialize(IOStream& stream) const;

				inline CodeBlock& GetCodeBlock() { return this->codeBlock; };
				inline CodeBlock const& GetCodeBlock() const { return this->codeBlock; };

				virtual String GetInstructionName() const { return GG_STR("While"); };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;

				virtual Instruction* Clone() const;
			};
		}
	}
}