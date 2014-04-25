#include <algorithm>

#include "MemoryManager.hpp"
#include "ManagedObject.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen::runtime;

MemoryManager::~MemoryManager()
{
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
		throw InternalErrorException("Attemted to register already registered object.");
	}
#endif

	if (this->nextObjectId == MAX_OBJECT_ID)
	{
		throw InternalErrorException("Reached maximum object ID.");
	}

	object->SetObjectId(this->nextObjectId);
	this->nextObjectId++;

	this->objects.push_back(object);
	object->AddRef(*this);
}

void MemoryManager::DestroyObject(ManagedObject* object)
{
	if (object->GetRefCount() != 0)
	{
		throw InternalErrorException("Can't release object with >0 references.");
	}

	this->objects.remove(object);
}

void MemoryManager::Serialize(std::iostream& stream) const
{
	for (const_iterator it = this->objects.begin(); it != this->objects.end(); it++)
	{
		(*it)->Serialize(stream);
		stream << ", " << (*it)->GetRefCount() << " refs" << std::endl;
	}
}