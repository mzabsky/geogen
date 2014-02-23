#include <string>

#include "../CodeLocation.hpp"
#include "DynamicObject.hpp"
#include "ReadOnlyWriteException.hpp"
#include "UndefinedSymbolAccessException.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

DynamicObject::DynamicObject(TypeDefinition const* type) : type(type)
{	
	/*for (SymbolDefinitionTable<VariableDefinition>::const_iterator it = type->GetVariableDefinitions().)
	for
	this->GetType()*/
}

void DynamicObject::SetMemberValue(VirtualMachine& vm, CodeLocation location, string memberName, DynamicObject* object)
{
	map<string, DynamicObject*>::iterator it = this->memberValues.find(memberName);
	if (it == this->memberValues.end())
	{
		VariableDefinition const* variableDefinition = this->GetType()->GetVariableDefinitions().GetItem(memberName);
		if (variableDefinition == NULL)
		{
			throw new UndefinedSymbolAccessException(GGE2203_UndefinedMemberVariable, location, memberName);
		}

		if (variableDefinition->IsConstant())
		{
			throw new ReadOnlyWriteException(location, memberName);
		}

		object->AddRef(vm);
		this->memberValues[memberName] = object;
	}
	else if (it->second != object)
	{
		it->second->RemoveRef(vm);
		object->AddRef(vm);

		it->second = object;
	}
}

DynamicObject* DynamicObject::GetMemberValue(VirtualMachine& vm, CodeLocation location, string memberName) const
{
	map<string, DynamicObject*>::const_iterator it = this->memberValues.find(memberName);
	if (it == this->memberValues.end())
	{
		VariableDefinition const* variableDefinition = this->GetType()->GetVariableDefinitions().GetItem(memberName);
		if (variableDefinition == NULL)
		{
			throw new UndefinedSymbolAccessException(GGE2203_UndefinedMemberVariable, location, memberName);
		}

		return NULL;
	}
	else
	{
		it->second->AddRef(vm);
		return it->second;
	}
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