#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "..\InternalErrorException.hpp"
#include "ArrayCountFunctionDefinition.hpp"
#include "ArrayEmptyFunctionDefinition.hpp"
#include "ArrayPushBackFunctionDefinition.hpp"
#include "ArrayFrontBackFunctionDefinition.hpp"
#include "ArrayGetFunctionDefinition.hpp"
#include "ArraySetFunctionDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ArrayTypeDefinition::ArrayTypeDefinition() : TypeDefinition("Array")
{	
	this->GetFunctionDefinitions().AddItem(new ArrayEmptyFunctionDefinition(this));

	this->GetFunctionDefinitions().AddItem(new ArrayCountFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArrayPushBackFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArrayGetFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArraySetFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(ArrayFrontBackFunctionDefinition::Create(ArrayFrontBackFunctionDefinition::FRONT, this));
	this->GetFunctionDefinitions().AddItem(ArrayFrontBackFunctionDefinition::Create(ArrayFrontBackFunctionDefinition::BACK, this));
		//this->GetFunctionDefinitions().AddItem()
}

ManagedObject* ArrayTypeDefinition::CreateInstance(VirtualMachine* vm) const
{
	ArrayObject* object = new ArrayObject(vm, this);
	vm->GetMemoryManager().RegisterObject(object);
	return object;
}

bool ArrayTypeDefinition::InstanceEqualsTo(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException("Using InstanceEqualsTo on object of incorrect type.");
	}

	if (b->GetType() == this)
	{
		ArrayObject const* arrayA = dynamic_cast<ArrayObject const*>(a);
		ArrayObject const* arrayB = dynamic_cast<ArrayObject const*>(b);
		
		if (arrayA->Count() != arrayB->Count())
		{
			return false;
		}

		ArrayObject::const_iterator itB = arrayB->Begin();
		for (ArrayObject::const_iterator itA = arrayA->Begin(); itA != arrayA->End(); itA++)
		{
			if (!itA->first->GetType()->InstanceEqualsTo(itA->first, itB->first))
			{
				return false;
			}

			if (!itA->second->GetType()->InstanceEqualsTo(itA->second, itB->second))
			{
				return false;
			}

			itB++;
		}
	}

	return TypeDefinition::InstanceLessThan(a, b);
}

bool ArrayTypeDefinition::InstanceLessThan(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException("Using InstanceLessThan on object of incorrect type.");
	}

	// TODO: Proper implementation

	/*if (b->GetType() == this)
	{
		return ((ArrayObject const*)a)->GetValue() < ((ArrayObject const*)b)->GetValue();
	}
	*/
	return TypeDefinition::InstanceLessThan(a, b);
}

ManagedObject* ArrayTypeDefinition::Copy(VirtualMachine* vm, runtime::ManagedObject* a) const
{
	return a;
}