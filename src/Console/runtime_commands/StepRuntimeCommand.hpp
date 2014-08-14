#pragma once

#include <vector>

#include "../RuntimeCommand.hpp"
#include "../Debugger.hpp"

namespace geogen
{
	namespace console
	{
		namespace runtime_commands
		{
			class StepRuntimeCommand : public RuntimeCommand
			{
			public:
				StepRuntimeCommand()
				{
					this->cues.push_back(GG_STR(""));
					this->cues.push_back(GG_STR("step"));
				}

				virtual String GetName() const { return GG_STR("Step"); };

				virtual String GetHelpString() const { return GG_STR("step - Advances by one instruction (default)."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetOut() << GG_STR("Step") << std::endl << std::endl;
					debugger->GetVirtualMachine()->Step();
					debugger->ShowCodeContext();

					runtime::instructions::Instruction const* currentInstruction = debugger->GetCurrentInstruction();
					debugger->GetOut() << std::endl << GG_STR("Next instruction: ") << (currentInstruction->ToString()) << GG_STR(" on line ") << currentInstruction->GetLocation().GetLine() << GG_STR(", column ") << currentInstruction->GetLocation().GetColumn() << GG_STR(". ") << std::endl << std::endl;
				}
			};
		}
	}
}