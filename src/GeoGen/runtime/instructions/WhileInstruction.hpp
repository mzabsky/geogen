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

				virtual void Serialize(std::iostream& stream) const;

				inline CodeBlock& GetCodeBlock() { return this->codeBlock; };
				inline CodeBlock const& GetCodeBlock() const { return this->codeBlock; };

				virtual std::string GetInstructionName() const { return "While"; };

				virtual InstructionStepResult Step(VirtualMachine* vm) const;
			};
		}
	}
}