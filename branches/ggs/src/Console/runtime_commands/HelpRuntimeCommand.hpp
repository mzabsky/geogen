#pragma once

#include <vector>

#include "../RuntimeCommand.hpp"
#include "../Debugger.hpp"

namespace geogen
{
	namespace console
	{
		namespace runtime_commands
		{
			class HelpRuntimeCommand : public RuntimeCommand
			{
			public:
				HelpRuntimeCommand()
				{
					this->cues.push_back(GG_STR("?"));
					this->cues.push_back(GG_STR("h"));
					this->cues.push_back(GG_STR("help"));
				}

				virtual String GetName() { return GG_STR("Help"); };

				virtual String GetHelpString() { return GG_STR("h - Displays the list of usable commands."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					for (CommandTable::iterator it = debugger->GetCommandTable().Begin(); it != debugger->GetCommandTable().End(); it++)
					{
						debugger->GetOut() << (*it)->GetHelpString() << std::endl;
					}
				}
			};
		}
	}
}