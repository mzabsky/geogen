#pragma once

#include <vector>
#include <fstream>


#include "../LoaderCommand.hpp"
#include "../Loader.hpp"
#include "../Debugger.hpp"
#include "../RendererDebugger.hpp"
#include "../ConsoleUtils.hpp"
#include "../../GeoGen/GeoGen.hpp"
#include "../../GeoGen/utils/StringUtils.hpp"

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

					Debugger debugger(loader->GetIn(), loader->GetOut(), *loader->GetCompiledScript(), geogen::runtime::ScriptParameters());

					try
					{
						debugger.Run();
					}
					catch (runtime::RuntimeException&)
					{
						loader->SetDump(debugger.GetVirtualMachine()->ToString());
						throw;
					}

					RendererDebugger rendererDebugger(loader->GetIn(), loader->GetOut(), debugger.GetVirtualMachine()->GetRenderingSequence());

					try
					{
						rendererDebugger.Run();
					}
					catch (runtime::RuntimeException&)
					{
						//loader->SetDump(debugger.GetVirtualMachine()->ToString());
						throw;
					}

					loader->GetOut() << "Saving maps." << std::endl;

					loader->SaveRenderedMaps(rendererDebugger.GetRenderer()->GetRenderedMapTable());
				}
			};
		}
	}
}