#include <algorithm>

#include "ArrayObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "InvalidOperationOnEmptyArrayException.hpp"
#include "ArrayKeyNotFoundException.hpp"
#include "ArrayIndexNotFoundException.hpp"
#include "NullKeyException.hpp"
#include "../random/RandomSequence.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::random;

ArrayObject::~ArrayObject()
{
	// Don't remove refs if the MM is already deleting everything anyways (could remove refs on already released object).
	if (this->GetMemberVariableTable().GetMemoryManager()->IsInCleanupMode())
	{
		return;
	}

	for (HashMap::iterator it = this->hashmap.begin(); it != this->hashmap.end(); it++)
	{
		it->first->RemoveRef(*this->GetMemberVariableTable().GetMemoryManager());
		it->second->RemoveRef(*this->GetMemberVariableTable().GetMemoryManager());
	}
}

ManagedObject* ArrayObject::Front(VirtualMachine* vm, CodeLocation location)
{
	if (this->Count() == 0)
	{
		throw InvalidOperationOnEmptyArrayException(location, GG_STR("Front"));
	}

	return this->Get(vm, location, *this->list.begin());
}

ManagedObject* ArrayObject::Back(VirtualMachine* vm, CodeLocation location)
{
	if (this->Count() == 0)
	{
		throw InvalidOperationOnEmptyArrayException(location, GG_STR("Back"));
	}

	return this->Get(vm, location, *this->list.rbegin());
}

ManagedObject* ArrayObject::Get(VirtualMachine* vm, CodeLocation location, ManagedObject* key)
{
	HashMap::iterator it = this->hashmap.find(key);
	if (it == this->hashmap.end())
	{
		throw ArrayKeyNotFoundException(location, key->GetStringValue());
	}

	return it->second;
}

ManagedObject* ArrayObject::GetActualKey(VirtualMachine* vm, CodeLocation location, ManagedObject* key)
{
	HashMap::iterator it = this->hashmap.find(key);
	if (it == this->hashmap.end())
	{
		throw ArrayKeyNotFoundException(location, key->GetStringValue());
	}

	return it->first;
}

void ArrayObject::Set(VirtualMachine* vm, CodeLocation location, ManagedObject* key, ManagedObject* value)
{
	if (key == vm->GetNull())
	{
		throw NullKeyException(location);
	}

	HashMap::iterator hashIt = this->hashmap.find(key);
	if (hashIt != this->hashmap.end())
	{
		hashIt->second->RemoveRef(vm->GetMemoryManager());
		hashIt->second = value;
	}
	else
	{
		this->hashmap[key] = value;
		key->AddRef();

		// Bump the max integer key if necessary
		if (key->GetType() == vm->GetNumberTypeDefinition())
		{
			NumberObject* numberKey = dynamic_cast<NumberObject*>(key);
			if (numberKey->GetValue() > this->maxIntegerKey)
			{
				this->maxIntegerKey = static_cast<int>(ceil(numberKey->GetValue()));
			}
		}

		this->list.push_back(key);
	}

	value->AddRef();
}

void ArrayObject::PopFront(runtime::VirtualMachine* vm, CodeLocation location)
{
	if (this->Count() == 0)
	{
		throw InvalidOperationOnEmptyArrayException(location, GG_STR("PopFront"));
	}

	iterator listIt = this->Begin();
	ManagedObject* key = *listIt;

	HashMap::iterator hashIt = this->hashmap.find(key);
	if (hashIt == this->hashmap.end())
	{
		throw InternalErrorException(GG_STR("Array internal consistency error - item found in list, but not in hash map during PopFront."));
	}

	hashIt->first->RemoveRef(vm->GetMemoryManager());
	hashIt->second->RemoveRef(vm->GetMemoryManager());

	this->hashmap.erase(hashIt);
	this->list.erase(listIt);
}

void ArrayObject::PopBack(runtime::VirtualMachine* vm, CodeLocation location)
{
	if (this->Count() == 0)
	{
		throw InvalidOperationOnEmptyArrayException(location, GG_STR("PopBack"));
	}

	iterator listIt = --this->End();
	ManagedObject* key = *listIt;

	HashMap::iterator hashIt = this->hashmap.find(key);
	if (hashIt == this->hashmap.end())
	{
		throw InternalErrorException(GG_STR("Array internal consistency error - item found in list, but not in hash map during PopBack."));
	}

	hashIt->first->RemoveRef(vm->GetMemoryManager());
	hashIt->second->RemoveRef(vm->GetMemoryManager());

	this->hashmap.erase(hashIt);
	this->list.erase(listIt);
}

void ArrayObject::PushBack(VirtualMachine* vm, CodeLocation location, ManagedObject* object)
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	ManagedObject* key = numberTypeDefinition->CreateInstance(vm, this->maxIntegerKey + 1);

	this->Set(vm, location, key, object);

	if (key->GetRefCount() < 1)
	{
		throw InternalErrorException(GG_STR("PushBack could not push object into the hashmap."));
	}
}

bool ArrayObject::ContainsKey(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key)
{
	return this->hashmap.find(key) != this->hashmap.end();
}

bool ArrayObject::ContainsValue(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* value)
{	
	for (HashMap::const_iterator it = this->hashmap.begin(); it != this->hashmap.end(); it++)
	{
		if (value->GetType()->InstanceEqualsTo(value, it->second))
		{
			return true;
		}
	}

	return false;
}

void ArrayObject::RemoveKey(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key)
{
	HashMap::iterator hashIt = this->hashmap.find(key);
	if (hashIt == this->hashmap.end())
	{
		throw ArrayKeyNotFoundException(location, key->GetStringValue());
	}

	List::iterator foundListIterator = this->End();
	for (iterator listIt = this->Begin(); listIt != this->End(); listIt++)
	{
		if (key->GetType()->InstanceEqualsTo(key, *listIt))
		{
			foundListIterator = listIt;
		}
	}

	if (foundListIterator == this->list.end())
	{
		throw InternalErrorException(GG_STR("Array internal consistency error - item found in hash map, but not in list during RemoveKey."));
	}

	this->list.erase(foundListIterator);

	hashIt->first->RemoveRef(vm->GetMemoryManager());
	hashIt->second->RemoveRef(vm->GetMemoryManager());
	this->hashmap.erase(hashIt);
}

void ArrayObject::RemoveValue(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* value)
{
	for (HashMap::iterator it = this->hashmap.begin(); it != this->hashmap.end(); it++)
	{
		if (value->GetType()->InstanceEqualsTo(value, it->second))
		{
			this->RemoveKey(vm, location, it->first);
			return;
		}
	}
}

ManagedObject* ArrayObject::GetKeyByIndex(runtime::VirtualMachine* vm, CodeLocation location, int index)
{
	if (index < 0 || (unsigned)index >= this->Count())
	{
		throw ArrayIndexNotFoundException(location, index);
	}

	List::iterator it = list.begin();
	advance(it, index);
	return *it;
}

ManagedObject* ArrayObject::GetValueByIndex(runtime::VirtualMachine* vm, CodeLocation location, int index)
{
	return this->Get(vm, location, this->GetKeyByIndex(vm, location, index));
}

void ArrayObject::SortByKeys(runtime::VirtualMachine* vm, CodeLocation location)
{
	this->list.clear();

	// The hash map is intrinsically ordered by keys
	for (HashMap::iterator it = this->hashmap.begin(); it != this->hashmap.end(); it++)
	{
		this->list.push_back(it->first);
	}
}

void ArrayObject::SortByValues(runtime::VirtualMachine* vm, CodeLocation location)
{
	class Sorter
	{
	public:
		VirtualMachine* vm;
		ArrayObject* array;
		bool operator() (ManagedObject* keyA, ManagedObject* keyB) 
		{ 
			ManagedObject* valueA = array->Get(this->vm, CodeLocation(0, 0), keyA);
			ManagedObject* valueB = array->Get(this->vm, CodeLocation(0, 0), keyB);
			return valueA->GetType()->InstanceLessThan(valueA, valueB);
		}
	};
	
	std::vector<ManagedObject*> working(this->list.begin(), this->list.end());

	Sorter sorter;
	sorter.vm = vm;
	sorter.array = this;

	sort(working.begin(), working.end(), sorter);
	
	this->list.clear();
	copy(working.begin(), working.end(), back_inserter(list));
}

void ArrayObject::Shuffle(runtime::VirtualMachine* vm, CodeLocation location, random::RandomSeed randomSeed)
{
	RandomSequence sequence(randomSeed);

	// Fisher-Yates shuffle
	vector<ManagedObject*> randomOrder(this->list.begin(), this->list.end());

	for (unsigned i = this->Count() - 1; i >= 0; i++)
	{
		unsigned j = sequence.NextUInt(0, this->Count() - 1);
		swap(randomOrder[i], randomOrder[j]);
	}

	this->list.clear();
	copy(randomOrder.begin(), randomOrder.end(), back_inserter(list));
}

String ArrayObject::GetStringValue() const
{
	StringStream ss;
	ss << "{" << endl;

	for (HashMap::const_iterator it = this->hashmap.begin(); it != this->hashmap.end(); it++)
	{
		ss << "\t{" << it->first->GetStringValue() << "}: {" << it->second->GetStringValue() << "}" << endl;
	}

	ss << "}";

	return ss.str();
}