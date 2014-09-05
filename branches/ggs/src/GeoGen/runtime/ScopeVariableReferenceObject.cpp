#include "ScopeVariableReferenceObject.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

ManagedObject* ScopeVariableReferenceObject::GetReferencedObject(CodeLocation location, VirtualMachine* vm)
{
	return this->variableTableItem->GetValue();
}

void ScopeVariableReferenceObject::SetReferencedObject(CodeLocation location, VirtualMachine* vm, ManagedObject* object)
{
	this->variableTableItem->SetValue(vm, object);
}

String ScopeVariableReferenceObject::GetStringValue() const
{
	return GG_STR("&[") + this->variableTableItem->GetValue()->GetStringValue() + GG_STR("]");
}