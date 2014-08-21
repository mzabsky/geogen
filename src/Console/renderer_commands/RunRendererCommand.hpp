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
			class RunRendererCommand : public RendererCommand
			{
			public:
				RunRendererCommand()
				{
					this->cues.push_back(GG_STR("r"));
					this->cues.push_back(GG_STR("run"));
				}

				virtual String GetName() const { return GG_STR("Run"); };

				virtual String GetHelpString() const { return GG_STR("run - Advances until the renderer finishes or fails."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					debugger->GetOut() << GG_STR("Run") << std::endl << std::endl;

					unsigned i = 1;
					while (debugger->GetRenderer()->GetStatus() == renderer::RENDERER_STATUS_READY)
					{
						renderer::RenderingStep const* currentStep = debugger->GetRenderer()->GetNextRenderingStep();
						debugger->GetOut() << i << GG_STR("/") << debugger->GetRenderer()->GetRenderingSequence().Size() << GG_STR(": ") << (currentStep->ToString()) << GG_STR(" on line ") << currentStep->GetLocation().GetLine() << GG_STR(", column ") << currentStep->GetLocation().GetColumn() << GG_STR(". ") << std::endl;

						debugger->Step();

						i++;
					}

					debugger->GetOut() << std::endl;
				}
			};
		}
	}
}