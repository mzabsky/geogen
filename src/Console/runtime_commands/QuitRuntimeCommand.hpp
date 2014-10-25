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
			class QuitRuntimeCommand : public RuntimeCommand
			{
			public:
				QuitRuntimeCommand()
				{
					this->cues.push_back(GG_STR("q"));
					this->cues.push_back(GG_STR("quit"));
				}

				virtual String GetName() const { return GG_STR("Quit"); };

				virtual String GetHelpString() const { return GG_STR("quit - Terminates the program."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					exit(0);
				}
			};
		}
	}
}