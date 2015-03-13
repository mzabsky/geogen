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