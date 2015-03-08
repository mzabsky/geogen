#include "VirtualMachine.hpp"
#include "ManagedObject.hpp"

using namespace std;
using namespace geogen::runtime;

bool VariableTableItem::SetValue(VirtualMachine* vm, ManagedObject* value)
{
	if (this->isConst)
	{
		return false;
	}

	if (value != this->value)
	{
		this->value->RemoveRef(vm->GetMemoryManager());
		value->AddRef();

		this->value = value;
	}

	return true;
}