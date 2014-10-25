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