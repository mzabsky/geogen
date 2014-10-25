#pragma once

#include <vector>

#include "../CodeLocation.hpp"
#include "../genlib/NoiseLayersFactory.hpp"
#include "../runtime/ManagedObjectHolder.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
		class ManagedObject;
	}

	namespace corelib
	{
		/// Transforms arguments to a Noise script function call to a geogen::genlib::NoiseLayers object.
		/// @param vm The virtual machine.
		/// @param location The code location of the call.
		/// @param arguments Call arguments.
		/// @return The layers object.
		genlib::NoiseLayers ParseNoiseInput(runtime::VirtualMachine* vm, CodeLocation location, std::vector<runtime::ManagedObject*> arguments);
	}
}