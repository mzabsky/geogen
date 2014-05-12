#include "MessageFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/StringTypeDefinition.hpp"
#include "..\InternalErrorException.hpp"
#include "MathDefinitionRangeException.hpp"
#include "..\runtime\UserErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

MessageFunctionDefinition* MessageFunctionDefinition::Create(Function function)
{
	switch (function)
	{
	case PRINT:
		return new MessageFunctionDefinition("Print", function);
	case TRIGGER_ERROR:
		return new MessageFunctionDefinition("TriggerError", function);
	default: throw InternalErrorException("Unknown method.");
	}
}

ManagedObject* MessageFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	StringTypeDefinition const* stringTypeDefinition = dynamic_cast<StringTypeDefinition const*>(vm->GetTypeDefinition("String"));

	vector<TypeDefinition const*> expectedParameterTypes;
	expectedParameterTypes.push_back(stringTypeDefinition);

	this->CheckArguments(location, expectedParameterTypes, arguments);

	string str = dynamic_cast<StringObject*>(arguments[0])->GetValue();
	
	switch (this->function)
	{
	case PRINT:
		if (vm->GetScriptMessageHandler() != NULL)
		{
			vm->GetScriptMessageHandler()(vm, location, str);
		}		
		break;
	case TRIGGER_ERROR:
		throw UserErrorException(location, str);
		break;
	default:
		throw InternalErrorException("Unknown method.");
	}

	return vm->GetNull();
}