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
			class ObjectStackRuntimeCommand : public RuntimeCommand
			{
			public:
				ObjectStackRuntimeCommand()
				{
					this->cues.push_back(GG_STR("s"));
					this->cues.push_back(GG_STR("os"));
					this->cues.push_back(GG_STR("stack"));
					this->cues.push_back(GG_STR("objectstack"));
				}

				virtual String GetName() const { return GG_STR("Object stack"); };

				virtual String GetHelpString() const { return GG_STR("s - Displays object stack."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetOut() << "Object stack: " << std::endl;
					debugger->GetOut() << debugger->GetVirtualMachine()->GetObjectStack().ToString() << std::endl;
				}
			};
		}
	}
}