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

#include "../runtime/ManagedObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
	}

	namespace corelib
	{
		/// Base class for reference objects.
		class ReferenceObject : public runtime::ManagedObject
		{
		private:
		protected:

			/// Constructor.
			/// @param vm The virtual machine.
			/// @param type The type definition.
			ReferenceObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type) : ManagedObject(vm, type) {};
		public:
			/// Destructor.
			virtual ~ReferenceObject() {};

			/// Gets the referenced object.
			/// @param location The location.
			/// @param vm The virtual machine
			/// @return The referenced object.
			virtual runtime::ManagedObject* GetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm) = 0;

			/// Sets the referenced object.
			/// @param location The location.
			/// @param vm The virtual machine.
			/// @param object The object to set as the reference target.
			virtual void SetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* object) = 0;
		};
	}
}