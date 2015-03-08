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
			class ReloadAndRunLoaderCommand : public LoaderCommand
			{
			public:
				ReloadAndRunLoaderCommand()
				{
					this->cues.push_back(GG_STR("rr"));
					this->cues.push_back(GG_STR("rlr"));
				}

				virtual String GetName() const { return GG_STR("Reload and run"); };

				virtual String GetHelpString() const { return GG_STR("rr - Reloads, recompiles and runs the last loaded script."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					loader->GetCommandQueue().push("rl");
					loader->GetCommandQueue().push("r");
				}
			};
		}
	}
}