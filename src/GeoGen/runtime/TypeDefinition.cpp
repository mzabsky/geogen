#include "../InternalErrorException.hpp"
#include "TypeDefinition.hpp"
#include "ManagedObject.hpp"
#include "StaticObject.hpp"
#include "VirtualMachine.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

void TypeDefinition::Initialize(VirtualMachine* vm) const
{
	ManagedObject* staticInstance = this->CreateStaticObject(vm);

	if (!vm->GetGlobalVariableTable().DeclareVariable(this->GetName(), staticInstance, true))
	{
		throw InternalErrorException(GG_STR("Could not declare static object variable (already initialized or name conflict?)."));
	}
}

StaticObject* TypeDefinition::CreateStaticObject(VirtualMachine* vm) const
{
	auto_ptr<StaticObject> object (new StaticObject(vm, this));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

bool TypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != b->GetType())
	{
		return a->GetType()->GetName() < b->GetType()->GetName();
	}
	
	return a->GetObjectId() < b->GetObjectId();
}

bool TypeDefinition::InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const
{
	return a == b;
}

bool TypeDefinition::IsConvertibleFrom(VirtualMachine* vm, TypeDefinition const* anotherTypeDefinition) const
{
	return this == anotherTypeDefinition;
}

ManagedObject* TypeDefinition::Convert(VirtualMachine* vm, ManagedObject* object) const
{
	if (this == object->GetType())
	{
		return object;
	}
	else 
	{
		throw InternalErrorException("Invalid type conversion.");
	}
}

void TypeDefinition::Serialize(IOStream& stream) const
{
	//stream << "{" << endl;

	if (this->GetFunctionDefinitions().Size() > 0)
	{
		stream << "FunctionDefinitions: " << endl;
		this->GetFunctionDefinitions().SerializeWithTabs(stream, 1);
	}

	if (this->GetStaticFunctionDefinitions().Size() > 0)
	{
		stream << "StaticFunctionDefinitions: " << endl;
		this->GetStaticFunctionDefinitions().SerializeWithTabs(stream, 1);
	}

	if (this->GetVariableDefinitions().Size() > 0)
	{
		stream << "VariableDefinitions: " << endl;
		this->GetVariableDefinitions().SerializeWithTabs(stream, 1);
	}

	if (this->GetStaticVariableDefinitions().Size() > 0)
	{
		stream << "StaticVariableDefinitions: " << endl;
		this->GetStaticVariableDefinitions().SerializeWithTabs(stream, 1);
	}

	//stream << "}" << endl;
}