#pragma once

#include "ReferenceObject.hpp"

namespace geogen
{
	namespace corelib
	{
		class ArrayObject;

		/// Implementation of a reference to Array item script object.
		class ArrayItemReferenceObject : public ReferenceObject
		{
		private:
			corelib::ArrayObject* array; 
			ManagedObject* key;
		public:

			/// Constructor.
			/// @param vm The virtual machine.
			/// @param type The type definition.
			/// @param array The array object.
			/// @param key The key object.
			ArrayItemReferenceObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type, ArrayObject* array, runtime::ManagedObject* key) : ReferenceObject(vm, type), array(array), key(key) {};
			//virtual ~ReferenceObject();

			virtual ManagedObject* GetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm);
			virtual void SetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* object);

			virtual String GetStringValue() const;
		};
	}
}