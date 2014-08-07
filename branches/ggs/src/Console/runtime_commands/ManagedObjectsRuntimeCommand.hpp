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
			class ManagedObjectsRuntimeCommand : public RuntimeCommand
			{
			public:
				ManagedObjectsRuntimeCommand()
				{
					this->cues.push_back(GG_STR("o"));
					this->cues.push_back(GG_STR("mo"));
					this->cues.push_back(GG_STR("mm"));
					this->cues.push_back(GG_STR("managedobjects"));
				}

				virtual String GetName() { return GG_STR("Managed objects"); };

				virtual String GetHelpString() { return GG_STR("o - Displays all existing managed objects."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetOut() << "Code block stack: " << std::endl;
					debugger->GetOut() << debugger->GetVirtualMachine()->GetMemoryManager().ToString() << std::endl;
				}
			};
		}
	}
}