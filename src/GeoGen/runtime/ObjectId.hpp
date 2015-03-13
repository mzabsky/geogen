/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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