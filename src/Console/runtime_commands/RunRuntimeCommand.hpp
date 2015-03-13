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
#include "../SignalHandler.hpp"

namespace geogen
{
	namespace console
	{
		namespace runtime_commands
		{
			class RunRuntimeCommand : public RuntimeCommand
			{
			public:
				RunRuntimeCommand()
				{
					this->cues.push_back(GG_STR("r"));
					this->cues.push_back(GG_STR("run"));
				}

				virtual String GetName() const { return GG_STR("Run"); };

				virtual String GetHelpString() const { return GG_STR("r - Advances until the virtual machine finishes or fails."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetOut() << GG_STR("Run") << std::endl << std::endl;

					while (debugger->GetVirtualMachine()->GetStatus() == runtime::VIRTUAL_MACHINE_STATUS_READY)
					{
						if (GetAndClearAbortFlag())
						{
							debugger->GetOut() << GG_STR("Aborted.") << std::endl;
							return;
						}

						debugger->GetVirtualMachine()->Step();
					}
				}
			};
		}
	}
}