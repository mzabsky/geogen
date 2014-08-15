#include "..\InternalErrorException.hpp"
#include "VariableDefinition.hpp"
#include "ManagedObject.hpp"

using namespace geogen;
using namespace runtime;
using namespace std;


void VariableDefinition::Initialize(VirtualMachine* vm) const
{
	ManagedObject* defaultValue = this->CreateDefaultValue();

	if (defaultValue != NULL)
	{
		vm->GetMemoryManager().RegisterObject(defaultValue);
	}

	if (!vm->GetGlobalVariableTable().DeclareVariable(this->GetName(), defaultValue, this->IsConstant()))
	{
		throw InternalErrorException(GG_STR("Could not declare static object variable (already initialized or name conflict?)."));
	}
}

void VariableDefinition::Serialize(IOStream& stream) const
{
	stream << (this->isConstant ? "const " : "") << this->name << endl;
}