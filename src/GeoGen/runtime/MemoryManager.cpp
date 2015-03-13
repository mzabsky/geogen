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

#include <algorithm>

#include "MemoryManager.hpp"
#include "ManagedObject.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

MemoryManager::~MemoryManager()
{
	// Don't let objects count refs anymode, they could touch already deleted object.
	this->cleanupMode = true;

	for (iterator it = this->objects.begin(); it != this->objects.end(); it++)
	{
		delete *it;
	}
}

void MemoryManager::RegisterObject(ManagedObject* object)
{
#ifdef DEBUG
	if (find(this->objects.begin(), this->objects.end(), object) != this->objects.end())
	{
		throw InternalErrorException(GG_STR("Attempted to register already registered object."));
	}
#endif

	if (this->nextObjectId == MAX_OBJECT_ID)
	{
		throw InternalErrorException(GG_STR("Reached maximum object ID."));
	}

	object->SetObjectId(this->nextObjectId);
	this->nextObjectId++;

	this->objects.push_back(object);
	//object->AddRef(*this);
}

void MemoryManager::DestroyObject(ManagedObject* object)
{
	if (object->GetRefCount() != 0)
	{
		throw InternalErrorException(GG_STR("Can't release object with >0 references."));
	}

#ifdef DEBUG
	if (find(this->objects.begin(), this->objects.end(), object) == this->objects.end())
	{
		throw InternalErrorException(GG_STR("Cannot remove unregistered object."));
	}
#endif

	this->objects.remove(object);

	delete object;
}

void MemoryManager::Serialize(IOStream& stream) const
{
	for (const_iterator it = this->objects.begin(); it != this->objects.end(); it++)
	{
		(*it)->Serialize(stream);
		stream << ", " << (*it)->GetRefCount() << " refs" << std::endl;
	}
}