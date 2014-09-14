#pragma once

#include <list>

#include "../Serializable.hpp"
#include "ObjectId.hpp"

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;

		/// Tracks objects and the references among them. It both uses ref-counting algorithm and holds a list of all objects so there are no memory leaks due to circular references. The actual ref-counts are stored in the objects themselves (and can be accessed using ManagedObject::AddRef, ManagedObject::RemoveRef and ManagedObject::GetRefCount). Formally, the managed owns all managed objects alive in the virtual machine - once the memory manager is destroyed, all the objects that are still alive will be destroyed too.
		class MemoryManager : public Serializable
		{
		private:
			typedef std::list<ManagedObject*>::iterator iterator;
			typedef std::list<ManagedObject*>::const_iterator const_iterator;

			std::list<ManagedObject*> objects;

			ObjectId nextObjectId;

			bool cleanupMode;

			MemoryManager(MemoryManager const& other) : nextObjectId(MIN_OBJECT_ID) {};
			MemoryManager& operator=(MemoryManager const&) {};
		public:
			/// Default constructor.
			MemoryManager() : nextObjectId(MIN_OBJECT_ID) {};

			/// Destructor. Destroys all tracked objects.
			virtual ~MemoryManager();

			/// Registers an object with the manager, transferring ownership of it there. Assigns ObjectId to the object.
			/// @param object The object.	
			void RegisterObject(ManagedObject* object);

			/// Destroys and object and unregisters its reference. Throw and exception if the object has non-zero ref count.
			/// @param [in,out] object If non-null, the object.
			void DestroyObject(ManagedObject* object);

			/// Determines whether the manager is in cleanup mode - its destructor was already called and all owned objects are being released. All other causes to release objects will be ignored (because the objects may be destroyed already).
			/// @return true if in cleanup mode, false if not.
			bool IsInCleanupMode() const { return this->cleanupMode; }

			virtual void Serialize(IOStream& stream) const;
		};
	}
}