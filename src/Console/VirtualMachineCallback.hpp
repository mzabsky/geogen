#pragma once

#include <GeoGen/GeoGen.hpp>

namespace geogen
{
	namespace console
	{
		static void VirtualMachineCallback(runtime::VirtualMachine* virtualMachine, CodeLocation location, String const& formattedMessage, String const& unformattedMessage, std::vector<String> arguments)
		{
			*((OStream*)virtualMachine->GetCallbackData()) << GG_STR("SCRIPT MESSAGE: ") << formattedMessage << std::endl;
		};
	}
}