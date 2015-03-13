/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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