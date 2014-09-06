#pragma once

#include <vector>

#include "Command.hpp"
#include "../GeoGen/renderer/Renderer.hpp"

namespace geogen
{
	namespace console
	{
		class RendererDebugger;

		class RendererCommand : public Command
		{
		public:
			virtual void Run(RendererDebugger* debugger, String arguments) const = 0;
		};
	}
} 