#pragma once

#include <vector>
#include <fstream>


#include "../LoaderCommand.hpp"
#include "../Loader.hpp"
#include "../ConsoleUtils.hpp"
#include "../../GeoGen/GeoGen.hpp"
#include "../../GeoGen/utils/StringUtils.hpp"

namespace geogen
{
	namespace console
	{
		namespace loader_commands
		{
			class RunLoaderCommand : public LoaderCommand
			{
			public:
				RunLoaderCommand()
				{
					this->cues.push_back(GG_STR("r"));
					this->cues.push_back(GG_STR("run"));
				}

				virtual String GetName() const { return GG_STR("Run"); };

				virtual String GetHelpString() const { return GG_STR("run - Runs a loaded script."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					if (loader->GetCompiledScript() == NULL)
					{
						loader->GetOut() << GG_STR("No script loaded. Use \"load\" command to load a script.") << std::endl << std::endl;
						return;
					}

					loader->GetOut() << "Runnning script." << std::endl << std::endl;

					runtime::VirtualMachine vm(*loader->GetCompiledScript(), runtime::ScriptParameters());
					vm.Run();
				}
			};
		}
	}
}