#pragma once

#include <string>
#include <list>

#include "../Serializable.hpp"
#include "ObjectId.hpp"

namespace geogen
{
	namespace runtime
	{
		class DynamicObject;

		class MemoryManager : public Serializable
		{
		private:
			typedef std::list<DynamicObject*>::iterator iterator;
			typedef std::list<DynamicObject*>::const_iterator const_iterator;

			std::list<DynamicObject*> objects;

			ObjectId nextObjectId;

			MemoryManager(MemoryManager const& other) {};
			MemoryManager& operator=(MemoryManager const&) {};
		public:
			MemoryManager() : nextObjectId(MIN_OBJECT_ID) {};
			virtual ~MemoryManager();

			void RegisterObject(DynamicObject* object);
			void DestroyObject(DynamicObject* object);

			virtual void Serialize(std::iostream& stream) const;
		};
	}
}