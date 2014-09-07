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