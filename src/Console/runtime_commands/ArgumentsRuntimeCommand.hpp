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
			class ArgumentsRuntimeCommand : public RuntimeCommand
			{
			public:
				ArgumentsRuntimeCommand()
				{
					this->cues.push_back(GG_STR("a"));
					this->cues.push_back(GG_STR("arg"));
					this->cues.push_back(GG_STR("args"));
					this->cues.push_back(GG_STR("arguments"));
				}

				virtual String GetName() const { return GG_STR("Arguments"); };

				virtual String GetHelpString() const { return GG_STR("args - Displays VM arguments."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetOut() << "Arguments: " << std::endl;
					StringStream ss;
					debugger->GetVirtualMachine()->GetArguments().SerializeWithTabs(ss, 1);
					debugger->GetOut() << ss.str() << std::endl;
				}
			};
		}
	}
}