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
			class RenderingSequenceRendererCommand : public RendererCommand
			{
			public:
				RenderingSequenceRendererCommand()
				{
					this->cues.push_back(GG_STR("rs"));
					this->cues.push_back(GG_STR("seq"));
					this->cues.push_back(GG_STR("rseq"));
					this->cues.push_back(GG_STR("renseq"));
					this->cues.push_back(GG_STR("renderingsequence"));
				}

				virtual String GetName() const { return GG_STR("Rendering sequence"); };

				virtual String GetHelpString() const { return GG_STR("rs - Displays the rendering sequence."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					debugger->GetOut() << GG_STR("Rendering sequence (scale ") << debugger->GetRenderer()->GetRenderingSequence().GetRenderScale() << GG_STR("):") << std::endl;
					debugger->GetOut() << debugger->GetRenderer()->GetRenderingSequence().ToString() << std::endl;
				}
			};
		}
	}
}