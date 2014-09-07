#pragma once

#include "../runtime/ManagedObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
	}

	namespace corelib
	{
		/// Base class for reference objects.
		class ReferenceObject : public runtime::ManagedObject
		{
		private:
		protected:

			/// Constructor.
			/// @param vm The virtual machine.
			/// @param type The type definition.
			ReferenceObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type) : ManagedObject(vm, type) {};
		public:
			/// Destructor.
			virtual ~ReferenceObject() {};

			/// Gets the referenced object.
			/// @param location The location.
			/// @param vm The virtual machine
			/// @return The referenced object.
			virtual runtime::ManagedObject* GetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm) = 0;

			/// Sets the referenced object.
			/// @param location The location.
			/// @param vm The virtual machine.
			/// @param object The object to set as the reference target.
			virtual void SetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* object) = 0;
		};
	}
}