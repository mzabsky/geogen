#include "CoordinateTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"
#include "CoordinateFromNumberFunctionDefinition.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

CoordinateTypeDefinition::CoordinateTypeDefinition() : TypeDefinition(GG_STR("Coordinate")) 
{
	this->GetStaticFunctionDefinitions().AddItem(new CoordinateFromNumberFunctionDefinition(this));
}


ManagedObject* CoordinateTypeDefinition::CreateInstance(VirtualMachine* vm, Coordinate value) const
{
	auto_ptr<ManagedObject> object(new CoordinateObject(vm, this, value));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

bool CoordinateTypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceLessThan on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<CoordinateObject const*>(a)->GetValue() < dynamic_cast<CoordinateObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceLessThan(a, b);
}

bool CoordinateTypeDefinition::InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceEqualsTo on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<CoordinateObject const*>(a)->GetValue() == dynamic_cast<CoordinateObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

ManagedObject* CoordinateTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using Copy on object of incorrect type."));
	}

	if (a->IsStaticObject())
	{
		return a;
	}

	return CreateInstance(vm, dynamic_cast<CoordinateObject const*>(a)->GetValue());
}