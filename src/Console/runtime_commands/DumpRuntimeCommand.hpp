#pragma once

#include <vector>
#include <fstream>


#include "../RuntimeCommand.hpp"
#include "../Debugger.hpp"

namespace geogen
{
	namespace console
	{
		namespace runtime_commands
		{
			class DumpRuntimeCommand : public RuntimeCommand
			{
			public:
				DumpRuntimeCommand()
				{
					this->cues.push_back(GG_STR("dump"));
				}

				virtual String GetName() const { return GG_STR("Dump"); };

				virtual String GetHelpString() const { return GG_STR("dump [file] - Dump current state of the virtual machine to a file (defaut = dump.txt)."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					if (arguments == GG_STR(""))
					{
						arguments = GG_STR("dump.txt");
					}

					String filename = debugger->GetOutputDirectory() + GG_STR("/") + arguments;

					OFStream dumpStream(filename);
					dumpStream << debugger->GetVirtualMachine()->ToString();
					dumpStream.flush();

					if (dumpStream.fail())
					{
						debugger->GetOut() << GG_STR("Could not write dump file.") << std::endl << std::endl;
					}
					else
					{
						debugger->GetOut() << GG_STR("Saved dump file \"") << filename << GG_STR("\".") << std::endl << std::endl;
					}
				}
			};
		}
	}
}