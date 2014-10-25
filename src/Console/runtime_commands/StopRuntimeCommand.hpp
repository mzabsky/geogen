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
			class StopRuntimeCommand : public RuntimeCommand
			{
			public:
				StopRuntimeCommand()
				{
					this->cues.push_back(GG_STR("stop"));
					this->cues.push_back(GG_STR("abort"));
				}

				virtual String GetName() const { return GG_STR("Stop"); };

				virtual String GetHelpString() const { return GG_STR("stop - Stop the debugger."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->Abort();
				}
			};
		}
	}
}