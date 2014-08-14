#pragma once

#include <vector>

#include "../RuntimeCommand.hpp"
#include "../Debugger.hpp"

namespace geogen
{
	namespace console
	{
		namespace runtime_commands
		{
			class RunRuntimeCommand : public RuntimeCommand
			{
			public:
				RunRuntimeCommand()
				{
					this->cues.push_back(GG_STR("r"));
					this->cues.push_back(GG_STR("run"));
				}

				virtual String GetName() { return GG_STR("Run"); };

				virtual String GetHelpString() { return GG_STR("r - Advances until the virtual machine finishes or fails."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetVirtualMachine()->Run();
				}
			};
		}
	}
}