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


					clock_t startTime = clock();

					loader->GetOut() << "Runnning script." << std::endl;

					runtime::VirtualMachine vm(*loader->GetCompiledScript(), loader->CreateScriptParameters());
					vm.Run();
					
					loader->GetOut() << "Rendering." << std::endl;

					renderer::Renderer renderer(vm.GetRenderingSequence());
					renderer.CalculateMetadata();

					loader->GetOut() << GG_STR("0% ");

					int i = 0;
					while (renderer.GetStatus() == geogen::renderer::RENDERER_STATUS_READY)
					{
						renderer.Step();

						loader->GetOut() << round(renderer.GetProgress()*10)/10 << "% ";

						i++;
					}

					loader->GetOut() << std::endl;

					loader->GetOut() << "Saving maps." << std::endl;

					loader->SaveRenderedMaps(renderer.GetRenderedMapTable());

					double seconds = (double)(clock() - startTime) / (double)CLOCKS_PER_SEC;

					loader->GetOut() << "Finished in " << seconds << " seconds." << std::endl << std::endl;
				}
			};
		}
	}
}