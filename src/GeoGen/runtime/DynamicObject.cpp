#include "DynamicObject.hpp"

using namespace geogen::runtime;

DynamicObject::DynamicObject(TypeDefinition const* type) : type(type)
{	
}

void DynamicObject::AddRef(VirtualMachine& vm)
{
	this->refCount++;
}

void DynamicObject::RemoveRef(VirtualMachine& vm)
{
	this->refCount--;

	if (this->refCount <= 0)
	{
		vm.UnregisterObject(this);
		delete this;
	}
}