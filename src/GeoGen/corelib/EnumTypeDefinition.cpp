#include "EnumTypeDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../InternalErrorException.hpp"
#include "NumberTypeDefinition.hpp"
#include "../runtime/VariableDefinition.hpp"
#include "../CodeLocation.hpp"
#include "EnumFromNumberFunctionDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;
using namespace corelib;

EnumTypeDefinition::EnumTypeDefinition(String const& name, ValueDefinitions valueDefinitions) : TypeDefinition(name), valueDefinitions(valueDefinitions)
{
	if (valueDefinitions.size() == 0)
	{
		throw ApiUsageException(GG_STR("Can't declare enum with no enum values"));
	}

	this->GetStaticFunctionDefinitions().AddItem(new EnumFromNumberFunctionDefinition(this));
}

void EnumTypeDefinition::Initialize(VirtualMachine* vm) const
{
	TypeDefinition::Initialize(vm);

	ManagedObject* staticObject = vm->GetStaticInstance(this->GetName());
	if (staticObject == NULL)
	{
		throw InternalErrorException(GG_STR("Enum type not initialized properly (static instance missing)."));
	}

	for (ValueDefinitions::const_iterator it = this->valueDefinitions.begin(); it != this->valueDefinitions.end(); it++)
	{
		ManagedObject* instance = this->CreateInstance(vm, it->second);
		staticObject->GetMemberVariableTable().DeclareVariable(it->first, instance, true);
	}
}


ManagedObject* EnumTypeDefinition::CreateInstance(VirtualMachine* vm, Number value) const
{
	auto_ptr<ManagedObject> object(new NumberObject(vm, this, value));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

bool EnumTypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceEqualsTo on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<NumberObject const*>(a)->GetValue() == dynamic_cast<NumberObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

bool EnumTypeDefinition::InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceEqualsTo on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<NumberObject const*>(a)->GetValue() == dynamic_cast<NumberObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

ManagedObject* EnumTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using Copy on object of incorrect type."));
	}

	if (a->IsStaticObject())
	{
		return a;
	}

	return CreateInstance(vm, dynamic_cast<NumberObject const*>(a)->GetValue());
}

ManagedObject* EnumTypeDefinition::GetValueByInt(VirtualMachine* vm, int intValue) const
{
	for (ValueDefinitions::const_iterator it = this->valueDefinitions.begin(); it != this->valueDefinitions.end(); it++)
	{
		if (it->second == intValue){
			if (!vm->GetGlobalVariableTable().IsVariableDeclared(this->GetName())){
				throw InternalErrorException(GG_STR("Enum type missing."));
			}

			ManagedObject* staticObject = vm->GetGlobalVariableTable().GetVariable(this->GetName())->GetValue();

			if (!staticObject->GetMemberVariableTable().IsVariableDeclared(it->first)){
				throw InternalErrorException(GG_STR("Enum value missing."));
			}

			return staticObject->GetMemberVariableTable().GetVariable(it->first)->GetValue();
		}
	}

	return NULL;
}

bool EnumTypeDefinition::IsValueIntDefined(int intValue) const
{
	for (ValueDefinitions::const_iterator it = this->valueDefinitions.begin(); it != this->valueDefinitions.end(); it++)
	{
		if (it->second == intValue){
			return true;
		}
	}

	return false;
}

int EnumTypeDefinition::GetDefaultValueInt() const
{
	int leastAbsValue = std::numeric_limits<int>::max();
	int leastAbsKey = 0;
	for (ValueDefinitions::const_iterator it = this->valueDefinitions.begin(); it != this->valueDefinitions.end(); it++)
	{
		if (abs(it->second) < leastAbsValue)
		{
			leastAbsValue = abs(it->second);
			leastAbsKey = it->second;
		}
	}

	return leastAbsKey;
}


bool EnumTypeDefinition::IsValueStringDefined(String stringValue) const
{
	return this->valueDefinitions.find(stringValue) != this->valueDefinitions.end();
}