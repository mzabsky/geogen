#include "ArrayFrontBackFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "..\InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;


ArrayFrontBackFunctionDefinition* ArrayFrontBackFunctionDefinition::Create(Method method, TypeDefinition const* owningType)
{
	switch (method)
	{
	case FRONT:
		return new ArrayFrontBackFunctionDefinition("Front", method, owningType);
	case BACK:
		return new ArrayFrontBackFunctionDefinition("Back", method, owningType);
	default: throw InternalErrorException("Unknown method.");
	}
}

ManagedObject* ArrayFrontBackFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	this->CheckArguments(location, 0, arguments);

	ArrayObject* thisArray = dynamic_cast<ArrayObject*>(instance);

	switch (this->method)
	{
	case FRONT:
		return thisArray->Front(vm, location);
	case BACK:
		return thisArray->Back(vm, location);
	default:
		throw InternalErrorException("Unknown method.");
	}
}