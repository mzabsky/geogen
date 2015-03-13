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
#include "../Point.hpp"
#include "../Number.hpp"
#include "../Direction.hpp"

namespace geogen
{
	namespace corelib
	{
		/// Implementation of Coordinate script object.
		class CoordinateObject : public runtime::ManagedObject
		{
		private:
			Number value;
			bool isRelative;
		public:

			/// Constructor.
			/// @param vm The virtual machine.
			/// @param type The type.
			/// @param value The value.
			/// @param isRelative True if the coordinate should be relative.
			CoordinateObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type, Number value, bool isRelative) : ManagedObject(vm, type), value(value), isRelative(isRelative) {};

			/// Gets the value.
			/// @return The value.
			inline Number GetValue() const { return this->value; }

			/// Determines whether the coordinate is relative.
			/// @return true if the coordinate is relative, false otherwise.
			inline bool IsRelative() const { return this->isRelative; }

			/// Gets the coordinate converted to absolute coordinate (if the coordinate wasn't relative, it will be returned unchanged).
			/// @param vm The virtual machine.
			/// @param location The location.
			/// @param direction The direction.
			/// @return The absolute coordinate.
			Coordinate GetAbsoluteCoordinate(runtime::VirtualMachine* vm, CodeLocation location, Direction direction) const;

			virtual String GetStringValue() const
			{
				StringStream ss;
				ss << (this->IsRelative() ? "@" : "") << this->GetValue();

				return ss.str();
			}
		};
	}
}