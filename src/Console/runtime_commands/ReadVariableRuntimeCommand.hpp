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
			class ReadVariableRuntimeCommand : public RuntimeCommand
			{
			public:
				ReadVariableRuntimeCommand()
				{
					this->cues.push_back(GG_STR("read"));
					this->cues.push_back(GG_STR("rv"));
					this->cues.push_back(GG_STR("rvar"));
					this->cues.push_back(GG_STR("readvar"));
					this->cues.push_back(GG_STR("readvariable"));
				}

				virtual String GetName() const { return GG_STR("Read variable"); };

				virtual String GetHelpString() const { return GG_STR("read [name] - Read variable [name]."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
				    debugger->GetOut() << "Variable \"" << arguments << "\":" << std::endl;

					if (arguments == "")
					{
						debugger->GetOut() << "Variable name not specified" << std::endl;
					}
					else
					{
						geogen::runtime::VariableTableItem* variableTableItem = debugger->GetVirtualMachine()->FindVariable(AnyStringToString(arguments));
						if (variableTableItem == NULL)
						{
							debugger->GetOut() << "Undefined" << std::endl;
						}
						else
						{
							debugger->GetOut() << "{ " << variableTableItem->GetValue()->ToString() << " } " << (variableTableItem->IsConst() ? "const" : "") << std::endl;
						}
					}

					debugger->GetOut() << std::endl;
				}
			};
		}
	}
}