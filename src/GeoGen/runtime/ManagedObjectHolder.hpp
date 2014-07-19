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
			ManagedObjectHolder() : vm(NULL), object(NULL) {}
			ManagedObjectHolder(VirtualMachine* vm, ManagedObject* object);
			ManagedObjectHolder(ManagedObjectHolder const&);
			ManagedObjectHolder& operator=(ManagedObjectHolder const&);
			~ManagedObjectHolder();

			inline ManagedObject* GetObject();
		};
	}
}