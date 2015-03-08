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
			class CallStackRuntimeCommand : public RuntimeCommand
			{
			public:
				CallStackRuntimeCommand()
				{
					this->cues.push_back(GG_STR("cs"));
					this->cues.push_back(GG_STR("callstack"));
				}

				virtual String GetName() const  { return GG_STR("Call stack"); };

				virtual String GetHelpString() const { return GG_STR("cs - Displays call stack."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetOut() << "Call stack: " << std::endl;
					debugger->GetOut() << debugger->GetVirtualMachine()->GetCallStack().ToString() << std::endl;
				}
			};
		}
	}
}