#pragma once

#include "../runtime/ManagedObject.hpp"
#include "../Point.hpp"
#include "../Number.hpp"
#include "../Direction.hpp"

namespace geogen
{
	namespace corelib
	{
		class PointObject : public runtime::ManagedObject
		{
		private:
			Number x, y;
			bool isXRelative, isYRelative;
		public:
			PointObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type, Number x, Number y, bool isXRelative, bool isYRelative) : ManagedObject(vm, type), x(x), isXRelative(isXRelative), y(y), isYRelative(isYRelative) {};
			inline Number GetX() const { return this->x; }
			inline Number GetY() const { return this->y; }
			inline bool IsXRelative() const { return this->isXRelative; }
			inline bool IsYRelative() const { return this->isYRelative; }
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