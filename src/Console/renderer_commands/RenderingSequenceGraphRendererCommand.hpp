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
			class RenderingSequenceGraphRendererCommand : public RendererCommand
			{
			public:
				RenderingSequenceGraphRendererCommand()
				{
					this->cues.push_back(GG_STR("rsg"));
					this->cues.push_back(GG_STR("renseqg"));
					this->cues.push_back(GG_STR("graph"));
					this->cues.push_back(GG_STR("renseqgraph"));
					this->cues.push_back(GG_STR("renderingsequencegraph"));
				}

				virtual String GetName() const { return GG_STR("Rendering sequence graph"); };

				virtual String GetHelpString() const { return GG_STR("rsg - Displays the rendering sequence graph."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					debugger->GetOut() << GG_STR("Rendering sequence graph nodes: ") << std::endl;
					for (renderer::RenderingSequence::const_iterator it = debugger->GetRenderer()->GetRenderingSequence().Begin(); it != debugger->GetRenderer()->GetRenderingSequence().End(); it++)
					{
						debugger->GetOut() << GG_STR("#") << debugger->GetRenderer()->GetRenderingSequenceMetadata().GetStepNumberByAddress(*it) << GG_STR(" - ") << (*it)->ToString() << std::endl;
						debugger->GetOut() << GG_STR("\tF: ");

						for (renderer::RenderingGraphNode::iterator it2 = debugger->GetRenderer()->GetRenderingGraph().GetNodeByStep(*it)->ForwardBegin(); it2 != debugger->GetRenderer()->GetRenderingGraph().GetNodeByStep(*it)->ForwardEnd(); it2++)
						{
							debugger->GetOut() << debugger->GetRenderer()->GetRenderingSequenceMetadata().GetStepNumberByAddress((*it2)->GetStep());

							if (it2 + 1 != debugger->GetRenderer()->GetRenderingGraph().GetNodeByStep(*it)->ForwardEnd())
							{
								debugger->GetOut() << GG_STR(", ");
							}
						}

						debugger->GetOut() << std::endl;
						debugger->GetOut() << GG_STR("\tB: ");

						for (renderer::RenderingGraphNode::iterator it2 = debugger->GetRenderer()->GetRenderingGraph().GetNodeByStep(*it)->BackBegin(); it2 != debugger->GetRenderer()->GetRenderingGraph().GetNodeByStep(*it)->BackEnd(); it2++)
						{
							debugger->GetOut() << debugger->GetRenderer()->GetRenderingSequenceMetadata().GetStepNumberByAddress((*it2)->GetStep());

							if (it2 + 1 != debugger->GetRenderer()->GetRenderingGraph().GetNodeByStep(*it)->BackEnd())
							{
								debugger->GetOut() << GG_STR(", ");
							}
						}

						debugger->GetOut() << std::endl;
					}
				}
			};
		}
	}
}