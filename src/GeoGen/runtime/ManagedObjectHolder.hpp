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

#include <stddef.h>

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;
		class VirtualMachine;

		/// Holds a reference to an ManagedObject for as long as the holder exists.
		class ManagedObjectHolder
		{
		private:
			VirtualMachine* vm;
			ManagedObject* object;
		public:
			/// Constructs an empty holder.
			ManagedObjectHolder() : vm(NULL), object(NULL) {}

			/// Constructs a holder holding a reference to an object.
			/// @param vm The virtual machine.
			/// @param object The reference.
			ManagedObjectHolder(VirtualMachine* vm, ManagedObject* object);

			/// Copy constructor.
			/// @param other The other.
			ManagedObjectHolder(ManagedObjectHolder const&);

			/// Assignment operator.
			/// @param other The other.
			/// @return A copy of this holder.
			ManagedObjectHolder& operator=(ManagedObjectHolder const&);
			
			/// Destructor.
			~ManagedObjectHolder();

			/// Gets the held object.
			/// @return The object.
			inline ManagedObject* GetWrappedObject();
		};
	}
}