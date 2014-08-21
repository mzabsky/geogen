#pragma once

#include <vector>
#include <exception>

#include "../RendererCommand.hpp"
#include "../RendererDebugger.hpp"

namespace geogen
{
	namespace console
	{
		namespace renderer_commands
		{
			class AutosaveRendererCommand : public RendererCommand
			{
			public:
				AutosaveRendererCommand()
				{
					this->cues.push_back(GG_STR("as"));
					this->cues.push_back(GG_STR("autosave"));
				}

				virtual String GetName() const { return GG_STR("Autosave"); };

				virtual String GetHelpString() const { return GG_STR("as [status] - Change autosave status to 0/1 (0 default). Autosave saves the affected map after each step."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					if (arguments == GG_STR(""))
					{
						debugger->GetOut() << GG_STR("Autosave ") << (debugger->GetAutosave() ? GG_STR("is enabled.") : GG_STR("is disabled.")) << std::endl << std::endl;
						return;
					}

					StringStream ss(arguments);
					bool state;
					ss >> state;

					debugger->SetAutosave(state);

					debugger->GetOut() << GG_STR("Autosave ") << (debugger->GetAutosave() ? GG_STR("enabled.") : GG_STR("disabled.")) << std::endl << std::endl;
				}
			};
		}
	}
}