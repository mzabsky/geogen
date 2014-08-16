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
			class QuitRendererCommand : public RendererCommand
			{
			public:
				QuitRendererCommand()
				{
					this->cues.push_back(GG_STR("q"));
					this->cues.push_back(GG_STR("quit"));
				}

				virtual String GetName() const { return GG_STR("Quit"); };

				virtual String GetHelpString() const { return GG_STR("quit - Terminate the program."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					exit(0);
				}
			};
		}
	}
}