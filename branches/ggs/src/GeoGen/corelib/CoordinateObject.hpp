#pragma once

#include "../runtime/ManagedObject.hpp"
#include "../Point.hpp"
#include "../Number.hpp"
#include "../Orientation.hpp"

namespace geogen
{
	namespace corelib
	{
		class CoordinateObject : public runtime::ManagedObject
		{
		private:
			Number value;
			bool isRelative;
		public:
			CoordinateObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type, Number value, bool isRelative) : ManagedObject(vm, type), value(value), isRelative(isRelative) {};
			inline Number GetValue() const { return this->value; }
			inline bool IsRelative() const { return this->isRelative; }
			Coordinate GetAbsoluteCoordinate(runtime::VirtualMachine* vm, CodeLocation location, Orientation orientation);

			virtual String GetStringValue() const
			{
				StringStream ss;
				ss << (this->IsRelative() ? "@" : "") << this->GetValue();

				return ss.str();
			}
		};
	}
}