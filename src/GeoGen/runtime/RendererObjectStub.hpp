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

#include "ManagedObject.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Stub representing an geogen::renderer::RendererObject during execution of an
		/// VirtualMachine. This object doesn't carry any value on its own, it is uniquely
		/// defined by its address. This address can then be converted into an renderer object slot
		/// number using the geogen::renderer::RendererObjectSlotTable.
		class RendererObjectStub : public ManagedObject
		{
		public:

			/// Constructor.
			/// @param vm The virtual machine.
			/// @param type The type definition.
			RendererObjectStub(VirtualMachine* vm, TypeDefinition const* type) : ManagedObject(vm, type)
			{
			};

			virtual String GetStringValue() const
			{
				StringStream ss;
				ss << this;

				return ss.str();
			}
		};
	}
}