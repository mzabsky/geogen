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

			unsigned nextFreeSlot;
			std::map<void*, unsigned> table;
		public:

			/// Default constructor.
			RendererObjectSlotTable() : nextFreeSlot(0) {}

			/// Gets a slot number corresponding to an address. If no such number was assigned yet to that address, a new one will be assigned.</summary>
			/// @param The address.
			/// @return The object slot assigned to the address.
			unsigned GetObjectSlotByAddress(void* address);
		};
	}
}