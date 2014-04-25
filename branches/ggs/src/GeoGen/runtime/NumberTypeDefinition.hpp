#pragma once

#include "../Number.hpp"
#include "TypeDefinition.hpp"
#include "PrimitiveObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
		typedef PrimitiveObject<Number> NumberObject;

		class NumberTypeDefinition : public TypeDefinition
		{
		public:
			NumberTypeDefinition() : TypeDefinition("Number") {}

			virtual ManagedObject* CreateInstance(VirtualMachine* vm, Number value) const;

			virtual bool InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const;

			virtual ManagedObject* Copy(VirtualMachine* vm, ManagedObject* a) const;
		};
	}
}