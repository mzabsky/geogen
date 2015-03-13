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
#include "../Debugger.hpp"
#include "../RendererDebugger.hpp"
#include "../ConsoleUtils.hpp"
#include <GeoGen/GeoGen.hpp>

namespace geogen
{
	namespace console
	{
		namespace loader_commands
		{
			class DebugLoaderCommand : public LoaderCommand
			{
			public:
				DebugLoaderCommand()
				{
					this->cues.push_back(GG_STR("d"));
					this->cues.push_back(GG_STR("debug"));
				}

				virtual String GetName() const { return GG_STR("Debug"); };

				virtual String GetHelpString() const { return GG_STR("debug - Starts debugging a loaded script."); };

				virtual void Run(Loader* loader, String arguments) const
				{
					if (loader->GetCompiledScript() == NULL)
					{
						loader->GetOut() << GG_STR("No script loaded. Use \"load\" command to load a script.") << std::endl << std::endl;
						return;
					}
					
					loader->GetOut() << "Debugger started." << std::endl << std::endl;

					std::vector<String> codeLines = geogen::utils::StringToLines(loader->GetCompiledScript()->GetCode());

					String input = "";

					Debugger debugger(loader->GetIn(), loader->GetOut(), *loader->GetCompiledScript(), loader->CreateScriptParameters(), loader->GetOutputDirectory());

					try
					{
						debugger.Run();
					}
					catch (runtime::RuntimeException&)
					{
						loader->SetDump(debugger.GetVirtualMachine()->ToString());
						throw;
					}

					if (debugger.IsAborted())
					{
						return;
					}

					RendererDebugger rendererDebugger(loader->GetIn(), loader->GetOut(), debugger.GetVirtualMachine()->GetRenderingSequence(), loader->GetOutputDirectory());

					try
					{
						rendererDebugger.Run();
					}
					catch (runtime::RuntimeException&)
					{
						//loader->SetDump(debugger.GetVirtualMachine()->ToString());
						throw;
					}

					if (!rendererDebugger.IsAborted())
					{
						loader->GetOut() << "Saving maps." << std::endl;

						if (!loader->SaveRenderedMaps(rendererDebugger.GetRenderer()->GetRenderedMapTable())) return;
					}
				}
			};
		}
	}
}