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


#include "../LoaderCommand.hpp"
#include "../Loader.hpp"

namespace geogen
{
	namespace console
	{
		namespace loader_commands
		{
			class LoadLoaderCommand : public LoaderCommand
			{
			public:
				LoadLoaderCommand()
				{
					this->cues.push_back(GG_STR("l"));
					this->cues.push_back(GG_STR("load"));
				}

				virtual String GetName() const { return GG_STR("Load"); };

				virtual String GetHelpString() const { return GG_STR("load [file] - Loads and compiles a script."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					if (arguments == GG_STR(""))
					{
						loader->GetOut() << GG_STR("File name required.") << std::endl << std::endl;
						return;
					}

					IFStream readStream(arguments.c_str());

					std::string str((std::istreambuf_iterator<char>(readStream)),
						std::istreambuf_iterator<char>());

					if (readStream.fail())
					{
						loader->GetOut() << GG_STR("Could not read file \"") << arguments << GG_STR("\".") << std::endl << std::endl;
						return;
					}
					else 
					{
						loader->GetOut() << GG_STR("Loaded file \"") << arguments << GG_STR("\".") << std::endl;
					}

					loader->SetCompiledScript(arguments, loader->GetCompiler()->CompileScript(str));


					loader->GetOut() << GG_STR("Compiled script.") << std::endl << std::endl;
				}
			};
		}
	}
}