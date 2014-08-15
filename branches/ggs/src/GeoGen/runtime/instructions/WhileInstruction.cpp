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
			void WhileInstruction::Serialize(IOStream& stream) const
			{
				stream << "While" << std::endl;
				this->codeBlock.SerializeWithTabs(stream, 1);
			}

			InstructionStepResult WhileInstruction::Step(VirtualMachine* vm) const
			{
				vm->GetCallStack().Top().GetCodeBlockStack().Push(this->GetLocation(), &vm->GetMemoryManager(), this->GetCodeBlock(), true);

				return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
			}
		}
	}
}