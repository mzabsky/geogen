#include "RendererObjectSlotTable.hpp"
#include "../InternalErrorException.hpp"

using namespace geogen;
using namespace renderer;

unsigned RendererObjectSlotTable::GetObjectSlotByAddress(void* address)
{
	iterator it = this->table.find(address);
	if (it != this->table.end())
	{
		return it->second;
	}

	// Not in the table yet
	unsigned slot = nextFreeSlot;
	nextFreeSlot++;
	this->table.insert(std::pair<void*, unsigned>(address, slot));

	return slot;
}