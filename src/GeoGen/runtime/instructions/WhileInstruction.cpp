#include <ostream>
#include <sstream>

#include "WhileInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"

namespace geogen 
{
	namespace runtime
	{
		namespace instructions
		{
			void WhileInstruction::Serialize(std::iostream& stream) const
			{
				stream << "While" << std::endl;

				std::stringstream substream;
				this->codeBlock.Serialize(substream);
				
				stream << substream.str();
			}

			InstructionStepResult WhileInstruction::Step(VirtualMachine* vm) const
			{
				CodeBlockStackEntry codeBlockStackEntry(this->GetCodeBlock(), true);
				vm->GetCallStack().top().GetCodeBlockStack().push(codeBlockStackEntry);

				return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
			}
		}
	}
}