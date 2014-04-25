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

			virtual ManagedObject* CreateInstance(VirtualMachine* vm, bool value) const;

			virtual bool InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const;

			virtual ManagedObject* Copy(VirtualMachine* vm, ManagedObject* a) const;
		};
	}
}