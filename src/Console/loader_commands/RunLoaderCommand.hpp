/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include <vector>
#include <fstream>


#include "../LoaderCommand.hpp"
#include "../Loader.hpp"
#include "../SignalHandler.hpp"
#include "../VirtualMachineCallback.hpp"
#include "../ConsoleUtils.hpp"
#include <GeoGen/GeoGen.hpp>

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
					vm.SetCallbackData(&loader->GetOut());
					vm.SetScriptMessageHandler(VirtualMachineCallback);
					while (vm.GetStatus() == runtime::VIRTUAL_MACHINE_STATUS_READY)
					{
						if (GetAndClearAbortFlag())
						{
							loader->GetOut() << std::endl << GG_STR("Aborted.") << std::endl << std::endl;
							return;
						}

						vm.Run();
					}
					
					loader->GetOut() << "Rendering." << std::endl;

					renderer::Renderer renderer(vm.GetRenderingSequence());
					renderer.CalculateMetadata();

					loader->GetOut() << GG_STR("0% ");

					int i = 0;
					while (renderer.GetStatus() == geogen::renderer::RENDERER_STATUS_READY)
					{
						if (GetAndClearAbortFlag())
						{
							loader->GetOut() << std::endl << GG_STR("Aborted.") << std::endl << std::endl;
							return;
						}

						renderer.Step();

						loader->GetOut() << round(renderer.GetProgress()*10)/10 << "% " << std::flush;

						i++;
					}

					loader->GetOut() << std::endl;

					loader->GetOut() << "Saving maps." << std::endl;

					if (!loader->SaveRenderedMaps(renderer.GetRenderedMapTable())) return;

					double seconds = (double)(clock() - startTime) / (double)CLOCKS_PER_SEC;

					loader->GetOut() << "Finished in " << seconds << " seconds." << std::endl << std::endl;
				}
			};
		}
	}
}