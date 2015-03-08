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
			class ReloadLoaderCommand : public LoaderCommand
			{
			public:
				ReloadLoaderCommand()
				{
					this->cues.push_back(GG_STR("rl"));
					this->cues.push_back(GG_STR("re"));
					this->cues.push_back(GG_STR("reload"));
				}

				virtual String GetName() const { return GG_STR("Reload"); };

				virtual String GetHelpString() const { return GG_STR("reload - Reloads and recompiles last loaded script."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					IFStream readStream(loader->GetCurrentFileName());

					std::string str((std::istreambuf_iterator<char>(readStream)),
						std::istreambuf_iterator<char>());

					if (readStream.fail())
					{
						loader->GetOut() << GG_STR("Could not read file \"") << loader->GetCurrentFileName() << GG_STR("\".") << std::endl << std::endl;
						return;
					}
					else
					{
						loader->GetOut() << GG_STR("Loaded file \"") << loader->GetCurrentFileName() << GG_STR("\".") << std::endl;
					}

					loader->SetCompiledScript(loader->GetCurrentFileName(), loader->GetCompiler()->CompileScript(str));


					loader->GetOut() << GG_STR("Compiled script.") << std::endl << std::endl;
				}
			};
		}
	}
}