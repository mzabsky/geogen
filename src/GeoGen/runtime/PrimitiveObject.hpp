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
		///	Universal class for most managed object types representing primitive types (Number, String etc.).
		/// @tparam T Primitive native type being represented.
		template<class T>
		class PrimitiveObject : public ManagedObject
		{
		private:
			T value;			
		public:

			/// Constructor.
			/// @param vm The virtual machine.
			/// @param type The type definition.
			/// @param value The wrapped value.
			PrimitiveObject(VirtualMachine* vm, TypeDefinition const* type, T value) : ManagedObject(vm, type), value(value)
			{				
			};

			/// Gets the wrapped value.
			/// @return The wrapped  value.
			inline T GetValue() const { return this->value; }

			virtual String GetStringValue() const
			{
				StringStream ss;
				ss << this->GetValue();

				return ss.str();
			}
		};
	}
}