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
			class StepRendererCommand : public RendererCommand
			{
			public:
				StepRendererCommand()
				{
					this->cues.push_back(GG_STR(""));
					this->cues.push_back(GG_STR("step"));
				}

				virtual String GetName() const { return GG_STR("Step"); };

				virtual String GetHelpString() const { return GG_STR("step - Advances by one step (default)."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					debugger->GetOut() << GG_STR("Step") << std::endl;
					debugger->GetOut() << GG_STR("Executed in ") << debugger->GetLastStepTime() << GG_STR(" seconds.") << std::endl;
					debugger->Step();

					renderer::RenderingStep const* currentStep = debugger->GetRenderer()->GetNextRenderingStep();
					if (currentStep != NULL)
					{
						debugger->GetOut() << std::endl << GG_STR("Next step: ") << (currentStep->ToString()) << GG_STR(" on line ") << currentStep->GetLocation().GetLine() << GG_STR(", column ") << currentStep->GetLocation().GetColumn() << GG_STR(". ") << std::endl;
					}					

					debugger->GetOut() << std::endl;
				}
			};
		}
	}
}