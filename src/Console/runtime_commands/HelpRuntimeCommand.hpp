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
			class HelpRuntimeCommand : public RuntimeCommand
			{
			public:
				HelpRuntimeCommand()
				{
					this->cues.push_back(GG_STR("?"));
					this->cues.push_back(GG_STR("h"));
					this->cues.push_back(GG_STR("help"));
				}

				virtual String GetName() const { return GG_STR("Help"); };

				virtual String GetHelpString() const { return GG_STR("? - Displays the list of commands available in current context."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					for (CommandTable::iterator it = debugger->GetCommandTable().Begin(); it != debugger->GetCommandTable().End(); it++)
					{
						debugger->GetOut() << (*it)->GetHelpString() << std::endl;
					}

					debugger->GetOut() << std::endl;
				}
			};
		}
	}
}