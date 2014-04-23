#pragma once

#include <string>
#include <list>

#include "ObjectId.hpp"

namespace geogen
{
	namespace runtime
	{
		class DynamicObject;

		class MemoryManager
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
			~MemoryManager();

			void RegisterObject(DynamicObject* object);
			void DestroyObject(DynamicObject* object);
		};
	}
}