#pragma once

#include <stddef.h>

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;
		class VirtualMachine;

		/// Holds a reference to an ManagedObject for as long as the holder exists.
		class ManagedObjectHolder
		{
		private:
			VirtualMachine* vm;
			ManagedObject* object;
		public:
			/// Constructs an empty holder.
			ManagedObjectHolder() : vm(NULL), object(NULL) {}

			/// Constructs a holder holding a reference to an object.
			/// @param vm The virtual machine.
			/// @param object The reference.
			ManagedObjectHolder(VirtualMachine* vm, ManagedObject* object);

			/// Copy constructor.
			/// @param other The other.
			ManagedObjectHolder(ManagedObjectHolder const&);

			/// Assignment operator.
			/// @param other The other.
			/// @return A copy of this holder.
			ManagedObjectHolder& operator=(ManagedObjectHolder const&);
			
			/// Destructor.
			~ManagedObjectHolder();

			/// Gets the held object.
			/// @return The object.
			inline ManagedObject* GetWrappedObject();
		};
	}
}