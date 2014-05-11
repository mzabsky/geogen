#include "ArrayRemoveFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ArrayRemoveFunctionDefinition* ArrayRemoveFunctionDefinition::Create(Method method, TypeDefinition const* owningType)
{
	switch (method)
	{
	case REMOVE_KEY:
		return new ArrayRemoveFunctionDefinition("RemoveKey", method, owningType);
	case REMOVE_VALUE:
		return new ArrayRemoveFunctionDefinition("RemoveValue", method, owningType);
	default: throw InternalErrorException("Unknown method.");
	}
}

ManagedObject* ArrayRemoveFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	this->CheckArguments(location, 1, arguments);

	ArrayObject* thisArray = dynamic_cast<ArrayObject*>(instance);

	switch (this->method)
	{
	case REMOVE_KEY:
		thisArray->RemoveKey(vm, location, arguments[0]);
		break;
	case REMOVE_VALUE:
		thisArray->RemoveValue(vm, location, arguments[0]);
		break;
	default:
		throw InternalErrorException("Unknown method.");
	}

	return vm->GetNull();
}