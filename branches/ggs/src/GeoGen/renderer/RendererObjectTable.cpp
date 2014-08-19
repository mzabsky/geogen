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