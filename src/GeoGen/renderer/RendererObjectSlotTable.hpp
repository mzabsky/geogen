#pragma once

#include <map>

namespace geogen
{
	namespace renderer
	{
		/// A table used to assign addresses of any objects to RendererObjectTable slots.
		class RendererObjectSlotTable
		{
			typedef std::map<void*, unsigned>::iterator iterator;
			typedef std::map<void*, unsigned>::const_iterator const_iterator;

			unsigned nextFreeSlot = 0;
			std::map<void*, unsigned> table;
		public:
			/// Gets a slot number corresponding to an address. If no such number was assigned yet to that address, a new one will be assigned.</summary>
			/// @param The address.
			/// @return The object slot assigned to the address.
			unsigned GetObjectSlotByAddress(void* address);
		};
	}
}