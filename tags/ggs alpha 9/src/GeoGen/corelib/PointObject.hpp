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