#pragma once

#include "ManagedObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class StaticObject : public ManagedObject
		{
		public:
			StaticObject(VirtualMachine* vm, TypeDefinition const* type) : ManagedObject(vm, type) {};

			virtual bool IsStaticObject(){ return true; };

			virtual String GetStringValue() const
			{
				return GG_STR("Type");
			};
		};
	}
}