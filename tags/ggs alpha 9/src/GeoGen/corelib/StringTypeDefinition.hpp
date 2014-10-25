#pragma once

#include "../String.hpp"
#include "../runtime/TypeDefinition.hpp"
#include "../runtime/PrimitiveObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
	}

	namespace corelib
	{
		typedef runtime::PrimitiveObject<String> StringObject;

		class StringTypeDefinition : public runtime::TypeDefinition
		{
		public:
			StringTypeDefinition() : TypeDefinition(GG_STR("String")) {}

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm, String const& value) const;

			virtual bool InstanceLessThan(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}