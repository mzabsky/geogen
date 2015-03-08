#pragma once

#include "../runtime/PrimitiveObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
		class TypeDefinition;
	}

	namespace corelib
	{
		typedef runtime::PrimitiveObject<bool> BooleanObject;

		class BooleanTypeDefinition : public runtime::TypeDefinition
		{
		public:
			BooleanTypeDefinition() : TypeDefinition(GG_STR("Boolean")) {}

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm, bool value) const;

			virtual bool InstanceLessThan(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}