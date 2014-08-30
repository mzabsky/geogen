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