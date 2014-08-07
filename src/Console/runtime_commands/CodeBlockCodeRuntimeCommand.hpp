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
			class CodeBlockCodeRuntimeCommand : public RuntimeCommand
			{
			public:
				CodeBlockCodeRuntimeCommand()
				{
					this->cues.push_back(GG_STR("cbc"));
					this->cues.push_back(GG_STR("codeblockcode"));
				}

				virtual String GetName() { return GG_STR("Code block code"); };

				virtual String GetHelpString() { return GG_STR("cbc [n] - Displays code of n-th code block on the code block stack (0 = topmost = default)."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					geogen::runtime::CodeBlockStack& codeBlockStack = debugger->GetVirtualMachine()->GetCallStack().Top().GetCodeBlockStack();

					unsigned codeBlockNumber = 0;
					if (arguments != "")
					{
						codeBlockNumber = atoi(arguments.c_str());
					}

					if (codeBlockNumber < codeBlockStack.Size())
					{
						debugger->GetOut() << "Code block stack entry " << codeBlockNumber << ":" << std::endl;
						debugger->GetOut() << (*(codeBlockStack.RBegin() - codeBlockNumber))->GetCodeBlock().ToString() << std::endl;
					}
					else
					{
						debugger->GetOut() << "Incorrect code block stack entry number" << std::endl;
					}
				}
			};
		}
	}
}