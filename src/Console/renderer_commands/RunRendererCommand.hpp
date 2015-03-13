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
#include "../SignalHandler.hpp"

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
						if (GetAndClearAbortFlag())
						{
							debugger->Abort();
							debugger->GetOut() << GG_STR("Aborted.") << std::endl;
							return;
						}

						renderer::RenderingStep const* currentStep = debugger->GetRenderer()->GetNextRenderingStep();
						debugger->GetOut() << i << GG_STR("/") << debugger->GetRenderer()->GetRenderingSequence().Size() << GG_STR(": ") << (currentStep->ToString()) << GG_STR(" on line ") << currentStep->GetLocation().GetLine() << GG_STR(", column ") << currentStep->GetLocation().GetColumn() << GG_STR(".");

						debugger->Step();

						debugger->GetOut() << GG_STR(".. ") << debugger->GetLastStepTime() << GG_STR(" seconds.") << std::endl;

						i++;
					}

					debugger->GetOut() << std::endl;
				}
			};
		}
	}
}