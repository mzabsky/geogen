#pragma once

#include <limits>

namespace geogen
{
	namespace runtime
	{
		/// Numeric managed object identifier.
		typedef unsigned ObjectId;

		/// Default object ID for an object that wasn't registered with the MemoryManager.
		static const ObjectId UNASSIGNED_OBJECT_ID = 0;

		/// The maximum object ID.
		static const ObjectId MAX_OBJECT_ID = std::numeric_limits<ObjectId>::max();
		
		/// The minimum object ID. This is the first object ID assigned, subsequent IDs are then assigned sequentially by adding one each time.
		static const ObjectId MIN_OBJECT_ID = 1;
	}
}