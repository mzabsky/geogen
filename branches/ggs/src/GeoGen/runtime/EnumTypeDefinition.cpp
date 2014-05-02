#include "../InternalErrorException.hpp"
#include "EnumTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "VariableDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

EnumTypeDefinition::EnumTypeDefinition(CodeLocation location, std::string const& name, ValueDefinitions valueDefinitions) : TypeDefinition(name), valueDefinitions(valueDefinitions)
{
	for (ValueDefinitions::const_iterator it = valueDefinitions.begin(); it != valueDefinitions.end(); it++){
		VariableDefinition* variableDefinition = new VariableDefinition(it->second, true);
		if (!this->GetStaticVariableDefinitions().AddItem(variableDefinition)){
			delete variableDefinition;
			throw InternalErrorException("Bad list of enum values.");
		}
	}
}

ManagedObject* EnumTypeDefinition::CreateInstance(VirtualMachine* vm, Number value) const
{
	auto_ptr<ManagedObject> object(new NumberObject(vm, this, value));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();

	// TODO: naplnit membery ZDE!
}

bool EnumTypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException("Using InstanceEqualsTo on object of incorrect type.");
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
		throw InternalErrorException("Using InstanceEqualsTo on object of incorrect type.");
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
		throw InternalErrorException("Using Copy on object of incorrect type.");
	}

	if (a->IsStaticObject())
	{
		return a;
	}

	return CreateInstance(vm, dynamic_cast<NumberObject const*>(a)->GetValue());
}