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
			class LoadLoaderCommand : public LoaderCommand
			{
			public:
				LoadLoaderCommand()
				{
					this->cues.push_back(GG_STR("l"));
					this->cues.push_back(GG_STR("load"));
				}

				virtual String GetName() const { return GG_STR("Load"); };

				virtual String GetHelpString() const { return GG_STR("load [file] - Loads and compiles a script."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					if (arguments == GG_STR(""))
					{
						loader->GetOut() << GG_STR("File name required.") << std::endl << std::endl;
						return;
					}

					IFStream readStream(arguments);

					std::string str((std::istreambuf_iterator<char>(readStream)),
						std::istreambuf_iterator<char>());

					if (readStream.fail())
					{
						loader->GetOut() << GG_STR("Could not read file \"") << arguments << GG_STR("\".") << std::endl << std::endl;
						return;
					}
					else 
					{
						loader->GetOut() << GG_STR("Loaded file \"") << arguments << GG_STR("\".") << std::endl;
					}

					loader->SetCompiledScript(loader->GetCompiler()->CompileScript(str));


					loader->GetOut() << GG_STR("Compiled script.") << std::endl << std::endl;
				}
			};
		}
	}
}