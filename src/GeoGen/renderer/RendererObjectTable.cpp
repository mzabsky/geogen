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

#include "RendererObjectTable.hpp"
#include "../InternalErrorException.hpp"
#include "RendererObject.hpp"

using namespace geogen;
using namespace renderer;

RendererObjectTable::~RendererObjectTable()
{
	for (iterator it = this->table.begin(); it != this->table.end(); it++)
	{
		delete *it;
	}
}

RendererObject* RendererObjectTable::GetObjectBySlot(unsigned slot)
{
	if (slot >= this->GetSize())
	{
		throw InternalErrorException(GG_STR("Invalid renderer object slot number."));
	}

	return this->table[slot];
}

void RendererObjectTable::SetObject(unsigned slot, RendererObject* object)
{
	if (slot >= this->GetSize())
	{
		throw InternalErrorException(GG_STR("Invalid renderer object slot number."));
	}

	this->table[slot] = object;
}

void RendererObjectTable::ReleaseObject(unsigned slot)
{
	if (slot >= this->GetSize())
	{
		throw InternalErrorException(GG_STR("Invalid renderer object slot number."));
	}

	delete this->table[slot];
	this->table[slot] = NULL;
}

void RendererObjectTable::Serialize(IOStream& stream) const
{
	int i = 0;
	for (const_iterator it = this->Begin(); it != this->End(); it++)
	{
		stream << GG_STR("#") << i << GG_STR(": ");
		if (*it != NULL)
		{
			(*it)->Serialize(stream);
		}
		else
		{
			stream << GG_STR("NULL"); 
		}
		stream << std::endl;

		i++;
	}
}