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
			class HelpRendererCommand : public RendererCommand
			{
			public:
				HelpRendererCommand()
				{
					this->cues.push_back(GG_STR("?"));
					this->cues.push_back(GG_STR("h"));
					this->cues.push_back(GG_STR("help"));
				}

				virtual String GetName() const { return GG_STR("Help"); };

				virtual String GetHelpString() const { return GG_STR("h - Displays the list of usable commands."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					for (CommandTable::iterator it = debugger->GetCommandTable().Begin(); it != debugger->GetCommandTable().End(); it++)
					{
						debugger->GetOut() << (*it)->GetHelpString() << std::endl;
					}

					debugger->GetOut() << std::endl;
				}
			};
		}
	}
}