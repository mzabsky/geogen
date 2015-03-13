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
			class ReloadAndRunLoaderCommand : public LoaderCommand
			{
			public:
				ReloadAndRunLoaderCommand()
				{
					this->cues.push_back(GG_STR("rr"));
					this->cues.push_back(GG_STR("rlr"));
				}

				virtual String GetName() const { return GG_STR("Reload and run"); };

				virtual String GetHelpString() const { return GG_STR("rr - Reloads, recompiles and runs the last loaded script."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					loader->GetCommandQueue().push("rl");
					loader->GetCommandQueue().push("r");
				}
			};
		}
	}
}