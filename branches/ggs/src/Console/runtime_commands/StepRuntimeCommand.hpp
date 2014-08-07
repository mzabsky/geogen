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
			class StepRuntimeCommand : public RuntimeCommand
			{
			public:
				StepRuntimeCommand()
				{
					this->cues.push_back(GG_STR(""));
					this->cues.push_back(GG_STR("step"));
				}

				virtual String GetName() { return GG_STR("Step"); };

				virtual String GetHelpString() { return GG_STR("step - Advances by one instruction (default)."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetVirtualMachine()->Step();
				}
			};
		}
	}
}