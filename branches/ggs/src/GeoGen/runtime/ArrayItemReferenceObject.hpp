#pragma once

#include "ReferenceObject.hpp"

namespace geogen
{
	namespace corelib
	{
		class ArrayObject;
	}

	namespace runtime
	{
		class ArrayItemReferenceObject : public ReferenceObject
		{
		private:
			corelib::ArrayObject* array; 
			ManagedObject* key;
		public:
			ArrayItemReferenceObject(VirtualMachine* vm, TypeDefinition const* type, corelib::ArrayObject* array, ManagedObject* key) : ReferenceObject(vm, type), array(array), key(key) {};
			//virtual ~ReferenceObject();

			virtual ManagedObject* GetReferencedObject(CodeLocation location, VirtualMachine* vm);
			virtual void SetReferencedObject(CodeLocation location, VirtualMachine* vm, ManagedObject* object);

			virtual String GetStringValue() const;
		};
	}
}