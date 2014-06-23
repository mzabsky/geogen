#pragma once

#include <map>

namespace geogen
{
	namespace renderer
	{
		class RendererObjectSlotTable
		{
			typedef std::map<void*, unsigned>::iterator iterator;
			typedef std::map<void*, unsigned>::const_iterator const_iterator;

			unsigned nextFreeSlot = 0;
			std::map<void*, unsigned> table;
		public:
			unsigned GetObjectSlotByAddress(void* address);
		};
	}
}