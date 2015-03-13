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

#include "ReferenceObject.hpp"

namespace geogen
{
	namespace corelib
	{
		class ArrayObject;

		/// Implementation of a reference to Array item script object.
		class ArrayItemReferenceObject : public ReferenceObject
		{
		private:
			corelib::ArrayObject* array; 
			ManagedObject* key;
		public:

			/// Constructor.
			/// @param vm The virtual machine.
			/// @param type The type definition.
			/// @param array The array object.
			/// @param key The key object.
			ArrayItemReferenceObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type, ArrayObject* array, runtime::ManagedObject* key) : ReferenceObject(vm, type), array(array), key(key) {};
			//virtual ~ReferenceObject();

			virtual ManagedObject* GetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm);
			virtual void SetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* object);

			virtual String GetStringValue() const;
		};
	}
}