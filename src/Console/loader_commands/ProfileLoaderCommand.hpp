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
			class ProfileLoaderCommand : public LoaderCommand
			{
			public:
				ProfileLoaderCommand()
				{
					this->cues.push_back(GG_STR("prof"));
					this->cues.push_back(GG_STR("profile"));
				}

				virtual String GetName() const { return GG_STR("Profile"); };

				virtual String GetHelpString() const { return GG_STR("prof [n] - Profiles a script for [n] of iterations (10 if not set)."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					unsigned numberOfInterations = 10;
					if (arguments != GG_STR(""))
					{
						StringStream argumentsStream(arguments);

						String numberString;
						argumentsStream >> numberString;

						StringStream ss(numberString);
						ss >> numberOfInterations;

						if(numberOfInterations < 1)
						{
							numberOfInterations = 10;
						}
					}

					if (loader->GetCompiledScript() == NULL)
					{
						loader->GetOut() << GG_STR("No script loaded. Use \"load\" command to load a script.") << std::endl << std::endl;
						return;
					}

					loader->GetOut() << "Profiling script." << std::endl;

					double executionTotalSeconds = 0;
					double prerenderingTotalSeconds = 0;
					double renderingTotalSeconds = 0;
					std::vector<double> renderingStepTotalSeconds;					
					std::vector<std::string> renderingStepLabels;
					for (unsigned i = 0; i < numberOfInterations; i++)
					{
						loader->GetOut() << std::endl << "Iteration " << i << "." << std::endl;

						loader->GetOut() << "Running script." << std::endl;

						clock_t executionStartTime = clock();

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

						executionTotalSeconds += (double)(clock() - executionStartTime) / (double)CLOCKS_PER_SEC;

						loader->GetOut() << "Rendering." << std::endl;

						clock_t prerenderingStartTime = clock();

						renderer::Renderer renderer(vm.GetRenderingSequence());
						renderer.CalculateMetadata();

						prerenderingTotalSeconds += (double)(clock() - prerenderingStartTime) / (double)CLOCKS_PER_SEC;
						clock_t renderingStartTime = clock();

						if(i == 0)
						{
							for (renderer::RenderingSequence::iterator it = vm.GetRenderingSequence().Begin(); it != vm.GetRenderingSequence().End(); it++)
							{
								renderingStepTotalSeconds.push_back(0);
								renderingStepLabels.push_back((*it)->ToString());
							}
						}
						
						loader->GetOut() << GG_STR("0% ");

						int j = 0;
						while (renderer.GetStatus() == geogen::renderer::RENDERER_STATUS_READY)
						{
							if (GetAndClearAbortFlag())
							{
								loader->GetOut() << std::endl << GG_STR("Aborted.") << std::endl << std::endl;
								return;
							}

							clock_t renderingStepStartTime = clock();

							renderer.Step();

							renderingStepTotalSeconds[j] += (double)(clock() - renderingStepStartTime) / (double)CLOCKS_PER_SEC;

							loader->GetOut() << round(renderer.GetProgress() * 10) / 10 << "% " << std::flush;

							j++;
						}

						renderingTotalSeconds += (double)(clock() - renderingStartTime) / (double)CLOCKS_PER_SEC;

						loader->GetOut() << std::endl;
					}

					loader->GetOut() << std::endl;
					loader->GetOut() << "Profile finished." << std::endl;
					loader->GetOut() << "Execution mean time: " << (executionTotalSeconds / numberOfInterations) << " s" << std::endl;
					loader->GetOut() << "Prerendering mean time: " << (prerenderingTotalSeconds / numberOfInterations) << " s" << std::endl;
					loader->GetOut() << "Rendering mean time: " << (renderingTotalSeconds / numberOfInterations) << " s" << std::endl;
					loader->GetOut() << std::endl;
					loader->GetOut() << "Rendering step times:" << std::endl;

					for (unsigned j = 0; j < renderingStepTotalSeconds.size(); j++)
					{
						loader->GetOut() << "" << (round(renderingStepTotalSeconds[j] / renderingTotalSeconds * 10 * 100) / 10) << "%\t" << renderingStepLabels[j] << " " << (renderingStepTotalSeconds[j] / numberOfInterations) << " s" << std::endl;
					}
				}
			};
		}
	}
}