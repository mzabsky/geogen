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
			class CodeBlockStackRuntimeCommand : public RuntimeCommand
			{
			public:
				CodeBlockStackRuntimeCommand()
				{
					this->cues.push_back(GG_STR("cbs"));
					this->cues.push_back(GG_STR("codeblockstack"));
				}

				virtual String GetName() const { return GG_STR("Code block stack"); };

				virtual String GetHelpString() const { return GG_STR("cbs - Displays code block stack."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetOut() << "Code block stack: " << std::endl;
					debugger->GetOut() << debugger->GetVirtualMachine()->GetCallStack().Top().GetCodeBlockStack().ToString() << std::endl;
				}
			};
		}
	}
}