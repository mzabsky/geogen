#pragma once

#include <vector>

#include "../RendererCommand.hpp"
#include "../RendererDebugger.hpp"

namespace geogen
{
	namespace console
	{
		namespace renderer_commands
		{
			class ObjectTableRendererCommand : public RendererCommand
			{
			public:
				ObjectTableRendererCommand()
				{
					this->cues.push_back(GG_STR("ot"));
					this->cues.push_back(GG_STR("rot"));
					this->cues.push_back(GG_STR("objecttable"));
					this->cues.push_back(GG_STR("rendererobjecttable"));
				}

				virtual String GetName() const { return GG_STR("Renderer object table"); };

				virtual String GetHelpString() const { return GG_STR("ot - Displays the renderer object table."); };

				virtual void Run(RendererDebugger* debugger, String arguments) const
				{
					debugger->GetOut() << GG_STR("Renderer object table: ") << std::endl;
					debugger->GetOut() << debugger->GetRenderer()->GetObjectTable().ToString() << std::endl;
				}
			};
		}
	}
}