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

				virtual String GetName() const { return GG_STR("Help"); };

				virtual String GetHelpString() const { return GG_STR("? - Displays the list of commands available in current context."); };

				virtual void Run(Debugger* debugger, String arguments) const
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