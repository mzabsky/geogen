#include <ostream>
#include <sstream>

#include "CallBlockInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\DynamicObject.hpp"
#include "..\BooleanTypeDefinition.hpp"
#include "..\IncorrectTypeException.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\TypeDefinition.hpp"

namespace geogen
{
	namespace runtime
	{
		namespace instructions
		{
			void CallBlockInstruction::Serialize(std::iostream& stream) const
			{
				stream << "CallBlock" << std::endl;

				std::stringstream substream;
				this->codeBlock.Serialize(substream);

				stream << substream.str();
			}

			InstructionStepResult CallBlockInstruction::Step(VirtualMachine* vm) const
			{
				vm->GetCallStack().Top().GetCodeBlockStack().Push(this->GetLocation(), &vm->GetMemoryManager(), this->GetCodeBlock(), false);

				return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
			}
		}
	}
}