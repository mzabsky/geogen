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
			class CodeBlockStackRuntimeCommand : public RuntimeCommand
			{
			public:
				CodeBlockStackRuntimeCommand()
				{
					this->cues.push_back(GG_STR("cbs"));
					this->cues.push_back(GG_STR("codeblockstack"));
				}

				virtual String GetName() { return GG_STR("Code block stack"); };

				virtual String GetHelpString() { return GG_STR("cbs - Displays code block stack."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetOut() << "Code block stack: " << std::endl;
					debugger->GetOut() << debugger->GetVirtualMachine()->GetCallStack().Top().GetCodeBlockStack().ToString() << std::endl;
				}
			};
		}
	}
}