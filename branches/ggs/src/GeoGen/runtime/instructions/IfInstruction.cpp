#include <ostream>
#include <sstream>

#include "IfInstruction.hpp"
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
			void IfInstruction::Serialize(std::iostream& stream) const
			{
				stream << "If" << std::endl;

				std::stringstream substream1;
				this->ifBranchCodeBlock.Serialize(substream1);

				stream << substream1.str();

				std::stringstream substream2;
				this->elseBranchCodeBlock.Serialize(substream2);

				stream << substream2.str();
			}

			InstructionStepResult IfInstruction::Step(VirtualMachine* vm) const
			{
				if (vm->GetObjectStack().size() < 1)
				{
					throw InternalErrorException("Object stack should contain at least 1 object.");
				}

				DynamicObject* conditionObject = (BooleanObject*)vm->GetObjectStack().top();
				vm->GetObjectStack().pop();

				TypeDefinition const* boolTypeDefinition = vm->GetBooleanTypeDefinition();
				if (conditionObject->GetType() != boolTypeDefinition)
				{
					throw IncorrectTypeException(GGE2104_IncorrectConditionResultType, this->GetLocation(), boolTypeDefinition->GetName(), conditionObject->GetType()->GetName());
				}
				else if (conditionObject->IsStaticObject()){
					throw IncorrectTypeException(GGE2104_IncorrectConditionResultType, this->GetLocation(), boolTypeDefinition->GetName(), "Static");
				}

				if (dynamic_cast<BooleanObject*>(conditionObject)->GetValue())
				{
					vm->GetCallStack().Top().GetCodeBlockStack().Push(&vm->GetMemoryManager(), this->GetIfBranchCodeBlock(), false);
				}
				else
				{
					vm->GetCallStack().Top().GetCodeBlockStack().Push(&vm->GetMemoryManager(), this->GetElseBranchCodeBlock(), false);
				}
				
				return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
			}
		}
	}
}