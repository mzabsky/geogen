#pragma once

#include <vector>

#include "../LoaderCommand.hpp"
#include "../Loader.hpp"

namespace geogen
{
	namespace console
	{
		namespace loader_commands
		{
			class HelpLoaderCommand : public LoaderCommand
			{
			public:
				HelpLoaderCommand()
				{
					this->cues.push_back(GG_STR("?"));
					this->cues.push_back(GG_STR("h"));
					this->cues.push_back(GG_STR("help"));
				}

				virtual String GetName() const { return GG_STR("Help"); };

				virtual String GetHelpString() const { return GG_STR("h - Displays the list of usable commands."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					for (CommandTable::iterator it = loader->GetCommandTable().Begin(); it != loader->GetCommandTable().End(); it++)
					{
						loader->GetOut() << (*it)->GetHelpString() << std::endl;
					}

					loader->GetOut() << std::endl;
				}
			};
		}
	}
}