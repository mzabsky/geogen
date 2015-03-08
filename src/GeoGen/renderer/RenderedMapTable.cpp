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
