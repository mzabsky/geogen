#include <algorithm>

#include "MemoryManager.hpp"
#include "DynamicObject.hpp"
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

void MemoryManager::RegisterObject(DynamicObject* object)
{
#ifdef DEBUG
	if (find(this->objects.begin(), this->objects.end(), object) != this->objects.end())
	{
		throw InternalErrorException("Attemted to register already registered object.");
	}
#endif

	this->objects.push_back(object);
	object->AddRef(*this);
}

void MemoryManager::DestroyObject(DynamicObject* object)
{
	if (object->GetRefCount() != 0)
	{
		throw InternalErrorException("Can't release object with >0 references.");
	}

	this->objects.remove(object);
}