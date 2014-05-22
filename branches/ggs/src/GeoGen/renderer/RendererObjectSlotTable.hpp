#pragma once

#include <map>

namespace geogen
{
	namespace renderer
	{
		class RendererObjectSlotTable
		{
			std::map<void*, unsigned> table;
		public:
			unsigned GetObjectSlotByAddress(void* address);
		};
	}
}