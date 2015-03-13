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

#include "../RendererCommand.hpp"
#include "../RendererDebugger.hpp"

namespace geogen
{
	namespace console
	{
		namespace renderer_commands
		{
			class ObjectTableRendererCommand : public RendererCommand
			{
			public:
				ObjectTableRendererCommand()
				{
					this->cues.push_back(GG_STR("ot"));
					this->cues.push_back(GG_STR("rot"));
					this->cues.push_back(GG_STR("objecttable"));
					this->cues.push_back(GG_STR("rendererobjecttable"));
				}

				virtual String GetName() const { return GG_STR("Renderer object table"); };

				virtual String GetHelpString() const { return GG_STR("ot - Displays the renderer object table."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					debugger->GetOut() << GG_STR("Renderer object table: ") << std::endl;
					debugger->GetOut() << debugger->GetRenderer()->GetObjectTable().ToString() << std::endl;
				}
			};
		}
	}
}