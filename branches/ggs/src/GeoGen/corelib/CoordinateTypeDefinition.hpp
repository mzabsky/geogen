#pragma once

#include "../Point.hpp"
#include "../runtime/PrimitiveObject.hpp"

namespace geogen
{
	namespace corelib
	{
		typedef runtime::PrimitiveObject<Coordinate> CoordinateObject;

		class CoordinateTypeDefinition : public runtime::TypeDefinition
		{
		public:
			CoordinateTypeDefinition();

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm, Coordinate value) const;

			virtual bool InstanceLessThan(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}