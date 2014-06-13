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
		throw InternalErrorException(GG_STR("Memory corruption detected when deleting managed object (object already deleted?)"));
	}
#endif
};

void ManagedObject::Serialize(IOStream& stream) const
{
	stream << "#" << this->objectId << " " << this->type->GetName() << " {" << this->GetStringValue() << "}";
}

void ManagedObject::AddRef(/*MemoryManager& vm*/)
{
	if (this->GetObjectId() == 0) throw exception();

	this->refCount++;
}

void ManagedObject::RemoveRef(MemoryManager& vm)
{
	if (this->refCount == 0)
	{
		throw InternalErrorException(GG_STR("Can't reduce refcount of an object with no references."));
	}

	this->refCount--;

	if (this->refCount == 0)
	{
		vm.DestroyObject(this);
	}
}