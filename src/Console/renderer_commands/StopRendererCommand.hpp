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
			class StopRendererCommand : public RendererCommand
			{
			public:
				StopRendererCommand()
				{
					this->cues.push_back(GG_STR("stop"));
				}

				virtual String GetName() const { return GG_STR("Stop"); };

				virtual String GetHelpString() const { return GG_STR("stop - Stop the renderer debugger."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					debugger->Abort();
				}
			};
		}
	}
}