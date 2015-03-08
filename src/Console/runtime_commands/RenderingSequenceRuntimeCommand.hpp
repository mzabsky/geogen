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
			class RenderingSequenceRuntimeCommand : public RuntimeCommand
			{
			public:
				RenderingSequenceRuntimeCommand()
				{
					this->cues.push_back(GG_STR("rs"));
					this->cues.push_back(GG_STR("renderingsequence"));
				}

				virtual String GetName() const { return GG_STR("Rendering sequence"); };

				virtual String GetHelpString() const { return GG_STR("rs - Displays rendering sequence."); };

				virtual void Run(Debugger* debugger, String arguments) const
				{
					debugger->GetOut() << GG_STR("Rendering sequence (scale ") << debugger->GetVirtualMachine()->GetRenderingSequence().GetRenderScale() << GG_STR("):") << std::endl;
					debugger->GetOut() << debugger->GetVirtualMachine()->GetRenderingSequence().ToString() << std::endl;
				}
			};
		}
	}
}