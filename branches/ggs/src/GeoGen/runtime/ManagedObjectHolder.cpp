#include "ManagedObjectHolder.hpp"
#include "ManagedObject.hpp"
#include "VirtualMachine.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

ManagedObjectHolder::ManagedObjectHolder(VirtualMachine* vm, ManagedObject* object) : vm(vm), object(object) 
{
	this->object->AddRef();
};

ManagedObjectHolder::ManagedObjectHolder(ManagedObjectHolder const& other) 
{
	this->vm = other.vm;
	this->object = other.object;
	this->object->AddRef();
}

ManagedObjectHolder& ManagedObjectHolder::operator=(ManagedObjectHolder const& other)
{
	if (this->object != NULL)
	{
		this->object->RemoveRef(this->vm->GetMemoryManager());
	}

	this->vm = other.vm;
	this->object = other.object;
	this->object->AddRef();

	return *this;
}

ManagedObjectHolder::~ManagedObjectHolder()
{
	if (this->object != NULL)
	{
		this->object->RemoveRef(vm->GetMemoryManager());
	}
}