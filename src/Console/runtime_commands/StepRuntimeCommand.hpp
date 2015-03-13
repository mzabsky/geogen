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
					
					if (debugger->GetVirtualMachine()->GetStatus() == runtime::VIRTUAL_MACHINE_STATUS_FINISHED)
					{
						debugger->GetOut() << GG_STR("Virtual machine is finished. ") << std::endl << std::endl;
					}
					else
					{
						debugger->GetVirtualMachine()->Step();
						debugger->ShowCodeContext();

						if (debugger->GetVirtualMachine()->GetStatus() == runtime::VIRTUAL_MACHINE_STATUS_FINISHED)
						{
							debugger->GetOut() << std::endl << GG_STR("Virtual machine finished. ") << std::endl << std::endl;
						}
						else
						{
							runtime::instructions::Instruction const* currentInstruction = debugger->GetCurrentInstruction();
							debugger->GetOut() << std::endl << GG_STR("Next instruction: \"") << (currentInstruction->ToString()) << GG_STR("\" on line ") << currentInstruction->GetLocation().GetLine() << GG_STR(", column ") << currentInstruction->GetLocation().GetColumn() << GG_STR(". ") << std::endl << std::endl;
						}
					}
				}
			};
		}
	}
}