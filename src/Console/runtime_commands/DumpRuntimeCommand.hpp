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
#include <fstream>


#include "../RuntimeCommand.hpp"
#include "../Debugger.hpp"

namespace geogen
{
	namespace console
	{
		namespace runtime_commands
		{
			class DumpRuntimeCommand : public RuntimeCommand
			{
			public:
				DumpRuntimeCommand()
				{
					this->cues.push_back(GG_STR("dump"));
				}

				virtual String GetName() const { return GG_STR("Dump"); };

				virtual String GetHelpString() const { return GG_STR("dump [file] - Dump current state of the virtual machine to a file (defaut = dump.txt)."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					if (arguments == GG_STR(""))
					{
						arguments = GG_STR("dump.txt");
					}

					String filename = debugger->GetOutputDirectory() + GG_STR("/") + arguments;

					OFStream dumpStream(filename.c_str());
					dumpStream << debugger->GetVirtualMachine()->ToString();
					dumpStream.flush();

					if (dumpStream.fail())
					{
						debugger->GetOut() << GG_STR("Could not write dump file.") << std::endl << std::endl;
					}
					else
					{
						debugger->GetOut() << GG_STR("Saved dump file \"") << filename << GG_STR("\".") << std::endl << std::endl;
					}
				}
			};
		}
	}
}