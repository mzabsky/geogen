#pragma once

#include <vector>

#include "Command.hpp"

namespace geogen
{
	namespace console
	{
		class Loader;

		class LoaderCommand : public Command
		{
		public:
			virtual void Run(Loader* debugger, String arguments) const = 0;
		};
	}
}