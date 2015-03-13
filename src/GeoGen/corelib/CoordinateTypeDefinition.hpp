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

#include "../Point.hpp"
#include "../Number.hpp"
#include "../runtime/TypeDefinition.hpp"

namespace geogen
{
	namespace corelib
	{
		class CoordinateObject;

		class CoordinateTypeDefinition : public runtime::TypeDefinition
		{
		public:
			CoordinateTypeDefinition();

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm, Number value, bool isRelative) const;

			virtual bool InstanceLessThan(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;

			/// Checks if this type is convertible from another type.
			/// @param vm The virtual machine.
			/// @param anotherTypeDefinition The other type.
			/// @return true if convertible from the other type, false if not.
			virtual bool IsConvertibleFrom(runtime::VirtualMachine* vm, runtime::TypeDefinition const* anotherTypeDefinition) const;

			/// Converts an managed object of its type to this type.
			/// @param vm The virtual machine.
			/// @param object The object to be converted. Must be convertible from its type to this type.
			/// @return null The converted object. If the two types are the same, the input object will be
			/// returned without any changes.
			virtual runtime::ManagedObject* Convert(runtime::VirtualMachine* vm, runtime::ManagedObject* object) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}