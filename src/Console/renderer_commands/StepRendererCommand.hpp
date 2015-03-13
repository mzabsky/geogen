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