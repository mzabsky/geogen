#include <string>

#include "../CodeLocation.hpp"
#include "DynamicObject.hpp"
#include "ReadOnlyWriteException.hpp"
#include "UndefinedSymbolAccessException.hpp"
#include "MemoryManager.hpp"
#include "VariableDefinition.hpp"
#include "TypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

DynamicObject::DynamicObject(TypeDefinition const* type) : type(type)
{	
	/*for (SymbolDefinitionTable<VariableDefinition>::const_iterator it = type->GetVariableDefinitions().)
	for
	this->GetType()*/
}

bool DynamicObject::operator<(const DynamicObject* rhs)
{
	return this->GetType()->InstanceLessThan(this, rhs);
}

bool DynamicObject::SetMemberValue(VirtualMachine& vm, CodeLocation location, string memberName, DynamicObject* object)
{
	map<string, DynamicObject*>::iterator it = this->memberValues.find(memberName);
	if (it == this->memberValues.end())
	{
		VariableDefinition const* variableDefinition = this->GetType()->GetVariableDefinitions().GetItem(memberName);
		if (variableDefinition == NULL)
		{
			throw UndefinedSymbolAccessException(GGE2203_UndefinedMemberVariable, location, memberName);
		}

		/*if (variableDefinition->IsConstant())
		{
			throw ReadOnlyWriteException(location, memberName);
		}*/

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
}

DynamicObject* DynamicObject::GetMemberValue(VirtualMachine& vm, CodeLocation location, string memberName) const
{
	map<string, DynamicObject*>::const_iterator it = this->memberValues.find(memberName);
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
}

void DynamicObject::AddRef(MemoryManager& vm)
{
	this->refCount++;
}

void DynamicObject::RemoveRef(MemoryManager& vm)
{
	this->refCount--;

	if (this->refCount <= 0)
	{
		// TODO: DO correct work with the references everywhere
		//vm.DestroyObject(this);
	}
}