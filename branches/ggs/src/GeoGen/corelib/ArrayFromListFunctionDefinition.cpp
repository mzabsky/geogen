#include "ArrayFromListFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* ArrayFromListFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, vector<ManagedObject*> arguments) const
{	
	// Make sure the number of arguments is even
	this->CheckArguments(location, arguments.size() - arguments.size() % 2, arguments);

	ArrayTypeDefinition const* arrayTypeDefinition = dynamic_cast<ArrayTypeDefinition const*>(instance->GetType());
	ArrayObject* returnObject = dynamic_cast<ArrayObject*>(arrayTypeDefinition->CreateInstance(vm));

	for (int i = 0; i < arguments.size() / 2; i++)
	{
		ManagedObject* key = arguments[unsigned(i * 2 + 1)];
		ManagedObject* value = arguments[unsigned(i * 2)];

		VariableTableItem* unsetKeyVariable = vm->FindVariable(ArrayTypeDefinition::UNSET_KEY_VARIABLE_NAME);
		if (unsetKeyVariable == NULL)
		{
			throw InternalErrorException(GG_STR("Could not find unset key marker."));
		}

		if (key == unsetKeyVariable->GetValue())
		{
			returnObject->PushBack(vm, location, value);
		}
		else
		{
			returnObject->Set(vm, location, key, value);
		}
	}

	return returnObject;
}