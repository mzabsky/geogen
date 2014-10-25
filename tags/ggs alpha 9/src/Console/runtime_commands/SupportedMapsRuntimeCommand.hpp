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
			class SupportedMapsRuntimeCommand : public RuntimeCommand
			{
			public:
				SupportedMapsRuntimeCommand()
				{
					this->cues.push_back(GG_STR("sm"));
					this->cues.push_back(GG_STR("maps"));
					this->cues.push_back(GG_STR("supmaps"));
					this->cues.push_back(GG_STR("supportedmap"));
				}

				virtual String GetName() const  { return GG_STR("Supported maps"); };

				virtual String GetHelpString() const { return GG_STR("sm - Displays list of maps supported by the script."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetOut() << "Supported maps: " << std::endl;
					for (std::vector<String>::const_iterator it = debugger->GetVirtualMachine()->GetCompiledScript().GetSupportedMaps().begin(); it != debugger->GetVirtualMachine()->GetCompiledScript().GetSupportedMaps().end(); it++)
					{
						debugger->GetOut() << /*"\t" <<*/ *it << std::endl;
					}
				}
			};
		}
	}
}