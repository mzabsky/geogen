#pragma once

#include "VirtualMachine.hpp"
#include "TypeDefinition.hpp"
#include "VirtualMachine.hpp"
#include "PrimitiveObject.hpp"

namespace geogen
{
	namespace runtime
	{
		typedef PrimitiveObject<bool> BooleanObject;

		class BooleanTypeDefinition : public TypeDefinition
		{
		public:
			BooleanTypeDefinition() : TypeDefinition("Boolean") {}

			virtual DynamicObject* CreateInstance(VirtualMachine* vm, bool value) const;

			virtual bool InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const;
			virtual bool InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const;

			virtual DynamicObject* Copy(VirtualMachine* vm, DynamicObject* a) const;
		};
	}
}