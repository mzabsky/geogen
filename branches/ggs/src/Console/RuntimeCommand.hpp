#pragma once

#include <vector>

#include "Command.hpp"
#include "../GeoGen/runtime/VirtualMachine.hpp"

namespace geogen
{
	namespace console
	{
		class Debugger;

		class RuntimeCommand : public Command
		{
		public:
			virtual void Run(Debugger* debugger, String arguments) const = 0;
		};
	}
}