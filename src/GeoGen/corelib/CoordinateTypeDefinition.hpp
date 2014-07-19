#pragma once

#include "../Point.hpp"
#include "../Number.hpp"
#include "../runtime/TypeDefinition.hpp"

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

			/// Checks if this type is convertible from another type.
			/// @param vm The virtual machine.
			/// @param anotherTypeDefinition The other type.
			/// @return true if convertible from the other type, false if not.
			virtual bool IsConvertibleFrom(runtime::VirtualMachine* vm, runtime::TypeDefinition const* anotherTypeDefinition) const;

			/// Converts an managed object of its type to this type.
			/// @param vm The virtual machine.
			/// @param object The object to be converted. Must be convertible from its type to this type.
			/// @return null The converted object. If the two types are the same, the input object will be
			/// returned without any changes.
			virtual runtime::ManagedObject* Convert(runtime::VirtualMachine* vm, runtime::ManagedObject* object) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}