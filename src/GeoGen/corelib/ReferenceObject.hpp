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
		class ReferenceObject : public runtime::ManagedObject
		{
		private:
		protected:
			ReferenceObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type) : ManagedObject(vm, type) {};
		public:
			virtual ~ReferenceObject() {};

			virtual runtime::ManagedObject* GetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm) = 0;
			virtual void SetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* object) = 0;
		};
	}
}