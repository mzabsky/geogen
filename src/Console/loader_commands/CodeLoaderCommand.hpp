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

#include "../LoaderCommand.hpp"
#include "../Loader.hpp"
#include "../SignalHandler.hpp"

namespace geogen
{
	namespace console
	{
		namespace loader_commands
		{
			class CodeLoaderCommand : public LoaderCommand
			{
			public:
				CodeLoaderCommand()
				{
					this->cues.push_back(GG_STR("c"));
					this->cues.push_back(GG_STR("code"));
				}

				virtual String GetName() const { return GG_STR("Code"); };

				virtual String GetHelpString() const { return GG_STR("code [code] - Compiles argument code as a script. If not provided, will ask for code interactively."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					StringStream code;
					if (arguments == GG_STR(""))
					{
						loader->GetOut() << GG_STR("Enter code to compile, the press CTRL+C:") << std::endl;

						String line;
						while (std::getline(loader->GetIn(), line))
						{
							code << line << std::endl;
						}

						IgnoreNextSignal();
						loader->GetIn().clear();

						loader->GetOut() << std::endl << std::endl;
					}
					else 
					{
						code << arguments;
					}					

					loader->SetCompiledScript("", loader->GetCompiler()->CompileScript(code.str()));


					loader->GetOut() << GG_STR("Compiled script.") << std::endl << std::endl;
				}
			};
		}
	}
}