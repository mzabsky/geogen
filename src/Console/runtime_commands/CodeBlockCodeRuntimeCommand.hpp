/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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

				virtual String GetName() const { return GG_STR("Code block code"); };

				virtual String GetHelpString() const { return GG_STR("cbc [n] - Displays code of n-th code block on the code block stack (0 = topmost = default)."); };

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