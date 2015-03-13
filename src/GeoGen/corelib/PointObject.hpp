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
		/// Implementation of Point script object.
		class PointObject : public runtime::ManagedObject
		{
		private:
			Number x, y;
			bool isXRelative, isYRelative;
		public:

			/// Constructor.
			/// @param vm The virtual machine.
			/// @param type The type definition.
			/// @param x The X coordinate.
			/// @param y The Y coordinate.
			/// @param isXRelative true if the X coordinate is to be relative, false otherwise.
			/// @param isYRelative true if the Y coordinate is to be relative, false otherwise.
			PointObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type, Number x, Number y, bool isXRelative, bool isYRelative) : ManagedObject(vm, type), x(x), isXRelative(isXRelative), y(y), isYRelative(isYRelative) {};

			/// Gets the X coordinate.
			/// @return The x coordinate.
			inline Number GetX() const { return this->x; }
			
			/// Gets the Y coordinate.
			/// @return The Y coordinate.
			inline Number GetY() const { return this->y; }

			/// Determines whether the X coordinate is relative.
			/// @return True if the X coordinate is relative, false otherwise.
			inline bool IsXRelative() const { return this->isXRelative; }
			
			/// Determines whether the Y coordinate is relative.
			/// @return True if the Y coordinate is relative, false otherwise.
			inline bool IsYRelative() const { return this->isYRelative; }

			/// Gets the point with both coordinates converted to absolute coordinates (if the coordinate wasn't relative, it will be unchanged).
			/// @param vm The virtual machine.
			/// @param location The location.
			/// @return The point with absolute coordinates.
			Point GetAbsolutePoint(runtime::VirtualMachine* vm, CodeLocation location) const;

			virtual String GetStringValue() const
			{
				StringStream ss;
				ss << "[" << (this->IsXRelative() ? "@" : "") << this->GetX() << ", " << (this->IsYRelative() ? "@" : "") << this->GetY() << "]";

				return ss.str();
			}
		};
	}
}