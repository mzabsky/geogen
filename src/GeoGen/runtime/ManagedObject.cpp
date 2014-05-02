#include <string>

#include "../CodeLocation.hpp"
#include "ManagedObject.hpp"
#include "ReadOnlyWriteException.hpp"
#include "UndefinedSymbolAccessException.hpp"
#include "MemoryManager.hpp"
#include "VariableDefinition.hpp"
#include "TypeDefinition.hpp"
#include "..\InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

ManagedObject::ManagedObject(VirtualMachine* vm, TypeDefinition const* type) : type(type), memberVariableTable(&vm->GetMemoryManager()), objectId(UNASSIGNED_OBJECT_ID)
{	
	/*for (SymbolDefinitionTable<VariableDefinition>::const_iterator it = type->GetVariableDefinitions().)
	for
	this->GetType()*/
}

ManagedObject::~ManagedObject()
{
#ifdef DEBUG				
	if (this->magic != MANAGED_OBJECT_MAGIC)
	{
		throw InternalErrorException("Memory corruption detected when deleting managed object (object already deleted?)");
	}
#endif
};

bool ManagedObject::operator<(const ManagedObject* rhs)
{
	return this->GetType()->InstanceLessThan(this, rhs);
}

void ManagedObject::Serialize(std::iostream& stream) const
{
	stream << "#" << this->objectId << " " << this->type->GetName() << " {" << this->GetStringValue() << "}";
}

/*bool ManagedObject::SetMemberValue(VirtualMachine& vm, CodeLocation location, string memberName, ManagedObject* object)
{
	map<string, ManagedObject*>::iterator it = this->memberValues.find(memberName);
	if (it == this->memberValues.end())
	{
		VariableDefinition const* variableDefinition = this->GetType()->GetVariableDefinitions().GetItem(memberName);
		if (variableDefinition == NULL)
		{
			throw UndefinedSymbolAccessException(GGE2203_UndefinedMemberVariable, location, memberName);
		}

		//if (variableDefinition->IsConstant())
		//{
		//	throw ReadOnlyWriteException(location, memberName);
		//}

		object->AddRef(vm.GetMemoryManager());
		this->memberValues[memberName] = object;
	}
	else if (it->second != object)
	{
		it->second->RemoveRef(vm.GetMemoryManager());
		object->AddRef(vm.GetMemoryManager());

		it->second = object;
	}

	return true;
}*/
/*
ManagedObject* ManagedObject::GetMemberValue(VirtualMachine& vm, CodeLocation location, string memberName) const
{
	map<string, ManagedObject*>::const_iterator it = this->memberValues.find(memberName);
	if (it == this->memberValues.end())
	{
		VariableDefinition const* variableDefinition = this->GetType()->GetVariableDefinitions().GetItem(memberName);
		if (variableDefinition == NULL)
		{
			throw UndefinedSymbolAccessException(GGE2203_UndefinedMemberVariable, location, memberName);
		}

		return NULL;
	}
	else
	{
		it->second->AddRef(vm.GetMemoryManager());
		return it->second;
	}
}*/

void ManagedObject::AddRef(/*MemoryManager& vm*/)
{
	if (this->GetObjectId() == 0) throw exception();

	this->refCount++;
}

void ManagedObject::RemoveRef(MemoryManager& vm)
{
	if (this->refCount == 0)
	{
		throw InternalErrorException("Can't reduce refcount of an object with no references.");
	}

	this->refCount--;

	if (this->refCount == 0)
	{
		vm.DestroyObject(this);
	}
}