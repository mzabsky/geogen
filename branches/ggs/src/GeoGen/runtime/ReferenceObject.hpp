#pragma once

#include "ManagedObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class ReferenceObject : public ManagedObject
		{
		private:
		protected:
			ReferenceObject(VirtualMachine* vm, TypeDefinition const* type) : ManagedObject(vm,type) {};
		public:
			virtual ~ReferenceObject();

			virtual ManagedObject* GetReferencedObject(CodeLocation location, VirtualMachine* vm) = 0;
			virtual void SetReferencedObject(CodeLocation location, VirtualMachine* vm, ManagedObject* object) = 0;
		};
	}
}