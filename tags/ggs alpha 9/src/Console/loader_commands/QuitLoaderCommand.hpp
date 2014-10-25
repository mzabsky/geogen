#pragma once

#include <vector>
#include <fstream>


#include "../LoaderCommand.hpp"
#include "../Loader.hpp"

namespace geogen
{
	namespace console
	{
		namespace loader_commands
		{
			class QuitLoaderCommand : public LoaderCommand
			{
			public:
				QuitLoaderCommand()
				{
					this->cues.push_back(GG_STR("q"));
					this->cues.push_back(GG_STR("quit"));
				}

				virtual String GetName() const { return GG_STR("Quit"); };

				virtual String GetHelpString() const { return GG_STR("quit - Terminates the program."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					exit(0);
				}
			};
		}
	}
}