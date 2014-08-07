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
			class ReadVariableRuntimeCommand : public RuntimeCommand
			{
			public:
				ReadVariableRuntimeCommand()
				{
					this->cues.push_back(GG_STR("r"));
					this->cues.push_back(GG_STR("rv"));
					this->cues.push_back(GG_STR("rvar"));
					this->cues.push_back(GG_STR("readvar"));
					this->cues.push_back(GG_STR("readvariable"));
				}

				virtual String GetName() { return GG_STR("Read variable"); };

				virtual String GetHelpString() { return GG_STR("r [name] - Read variable [name]."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
				    debugger->GetOut() << "Variable \"" << arguments << "\":" << std::endl;

					if (arguments == "")
					{
						debugger->GetOut() << "Variable name not specified" << std::endl;
					}
					else
					{
						geogen::runtime::VariableTableItem* variableTableItem = debugger->GetVirtualMachine()->FindVariable(AnyStringToString(arguments));
						if (variableTableItem == NULL)
						{
							debugger->GetOut() << "Undefined" << std::endl;
						}
						else
						{
							debugger->GetOut() << "{ " << variableTableItem->GetValue()->ToString() << " } " << (variableTableItem->IsConst() ? "const" : "") << std::endl;
						}
					}
				}
			};
		}
	}
}