#include "ArrayObject.hpp"
#include "../runtime/NumberTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "InvalidOperationOnEmptyArrayException.hpp"
#include "ArrayKeyNotFoundException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ArrayObject::~ArrayObject()
{
	for (iterator it = this->Begin(); it != this->End(); it++)
	{
		it->first->RemoveRef(*this->GetMemberVariableTable().GetMemoryManager());
		it->second->RemoveRef(*this->GetMemberVariableTable().GetMemoryManager());
	}
}

ManagedObject* ArrayObject::Front(VirtualMachine* vm, CodeLocation location)
{
	if (this->Count() == 0)
	{
		throw InvalidOperationOnEmptyArrayException(location, "Front");
	}

	return this->array.begin()->second;
}

ManagedObject* ArrayObject::Back(VirtualMachine* vm, CodeLocation location)
{
	if (this->Count() == 0)
	{
		throw InvalidOperationOnEmptyArrayException(location, "Back");
	}

	return this->array.rbegin()->second;
}

ManagedObject* ArrayObject::Get(VirtualMachine* vm, CodeLocation location, ManagedObject* key)
{
	iterator it = this->array.find(key);
	if (it == this->End())
	{
		throw ArrayKeyNotFoundException(location, key->GetStringValue());
	}

	return it->second;
}

void ArrayObject::Set(VirtualMachine* vm, CodeLocation location, ManagedObject* key, ManagedObject* value)
{
	iterator it = this->array.find(key);
	if (it != this->End())
	{
		it->second->RemoveRef(vm->GetMemoryManager());
		it->second = value;
	}
	else
	{
		this->array[key] = value;
		key->AddRef();

		// Bump the max integer key if necessary
		if (value->GetType() == vm->GetNumberTypeDefinition())
		{
			NumberObject* numberKey = dynamic_cast<NumberObject*>(key);
			if (numberKey->GetValue() > this->maxIntegerKey)
			{
				this->maxIntegerKey = ceil(numberKey->GetValue());
			}
		}
	}

	value->AddRef();
}

void ArrayObject::PopFront(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* object)
{
	if (this->Count() == 0)
	{
		throw InvalidOperationOnEmptyArrayException(location, "PopFront");
	}

	iterator it = this->Begin();
	it->first->RemoveRef(vm->GetMemoryManager());
	it->second->RemoveRef(vm->GetMemoryManager());
	this->array.erase(it);
}

void ArrayObject::PopBack(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* object)
{
	if (this->Count() == 0)
	{
		throw InvalidOperationOnEmptyArrayException(location, "PopBack");
	}

	iterator it = --this->End();
	it->first->RemoveRef(vm->GetMemoryManager());
	it->second->RemoveRef(vm->GetMemoryManager());
	this->array.erase(it);
}

ManagedObject* ArrayObject::PushBack(VirtualMachine* vm, CodeLocation location, ManagedObject* object)
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	ManagedObject* key = numberTypeDefinition->CreateInstance(vm, this->maxIntegerKey + 1);

	this->Set(vm, location, key, object);

	if (key->GetRefCount() < 1)
	{
		throw InternalErrorException("PushBack could not push object into the array.");
	}
}

bool ArrayObject::ContainsKey(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* object)
{
	return this->array.find(object) != this->End();
}

bool ArrayObject::ContainsValue(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* object)
{	
	for (const_iterator it = this->Begin(); it != this->End(); it++)
	{
		if (object->GetType()->InstanceEqualsTo(object, it->second))
		{
			return true;
		}
	}

	return false;
}

void ArrayObject::Remove(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key)
{
	iterator it = this->array.find(key);
	if (it == this->End())
	{
		throw ArrayKeyNotFoundException(location, key->GetStringValue());
	}

	it->first->RemoveRef(vm->GetMemoryManager());
	it->second->RemoveRef(vm->GetMemoryManager());
	this->array.erase(it);
}

string ArrayObject::GetStringValue() const
{
	std::stringstream ss;
	ss << "{" << endl;

	for (const_iterator it = this->Begin(); it != this->End(); it++)
	{
		ss << "\t{" << it->first->GetStringValue() << "}: {" << it->second->GetStringValue() << "}" << endl;
	}

	ss << "}";

	return ss.str();
}