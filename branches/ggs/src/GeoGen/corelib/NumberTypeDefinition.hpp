#pragma once

#include "../Number.hpp"
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
		typedef runtime::PrimitiveObject<Number> NumberObject;

		class NumberTypeDefinition : public runtime::TypeDefinition
		{
		public:
			NumberTypeDefinition();

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm, Number value) const;

			virtual bool InstanceLessThan(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}