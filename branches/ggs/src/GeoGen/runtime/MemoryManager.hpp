#pragma once

#include <string>
#include <list>

#include "../Serializable.hpp"
#include "ObjectId.hpp"

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;

		class MemoryManager : public Serializable
		{
		private:
			typedef std::list<ManagedObject*>::iterator iterator;
			typedef std::list<ManagedObject*>::const_iterator const_iterator;

			std::list<ManagedObject*> objects;

			ObjectId nextObjectId;

			MemoryManager(MemoryManager const& other) : nextObjectId(MIN_OBJECT_ID) {};
			MemoryManager& operator=(MemoryManager const&) {};
		public:
			MemoryManager() : nextObjectId(MIN_OBJECT_ID) {};
			virtual ~MemoryManager();

			void RegisterObject(ManagedObject* object);
			void DestroyObject(ManagedObject* object);

			virtual void Serialize(IOStream& stream) const;
		};
	}
}