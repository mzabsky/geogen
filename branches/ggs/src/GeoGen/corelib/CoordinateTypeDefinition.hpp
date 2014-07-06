#pragma once

#include "../Point.hpp"
#include "../Number.hpp"
#include "../runtime/PrimitiveObject.hpp"

namespace geogen
{
	namespace corelib
	{
		class CoordinateObject;

		class CoordinateTypeDefinition : public runtime::TypeDefinition
		{
		public:
			CoordinateTypeDefinition();

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm, Number value, bool isRelative) const;

			virtual bool InstanceLessThan(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}