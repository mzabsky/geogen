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

#include "RenderedMapTable.hpp"
#include "../genlib/HeightMap.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;
using namespace genlib;

bool RenderedMapTable::RemoveItem(String name)
{
	iterator it = this->table.find(name);
	if (it == this->table.end())
	{
		return false;
	}

	delete it->second;
	this->table.erase(it);

	return true;
}

void RenderedMapTable::Clear()
{
	for (iterator it = this->Begin(); it != this->End(); it++)
	{
		delete it->second;
	}

	this->table.clear();
}

void RenderedMapTable::Serialize(IOStream& stream) const
{
	for (RenderedMapTable::const_iterator it = this->Begin(); it != this->End(); it++)
	{
		stream << it->first << GG_STR(": ") << it->second->GetRectangle().ToString() << GG_STR(" at address 0x") << it->second << endl;
	}
}
