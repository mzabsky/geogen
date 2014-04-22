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

			virtual DynamicObject* CreateInstance(VirtualMachine* vm, Number value) const;

			virtual bool InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const;
			virtual bool InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const;

			virtual DynamicObject* Copy(VirtualMachine* vm, DynamicObject* a) const;
		};
	}
}