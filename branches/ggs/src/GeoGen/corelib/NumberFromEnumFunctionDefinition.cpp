#include "NumberFromEnumFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "..\runtime\IncorrectTypeException.hpp"
#include "NumberTypeDefinition.hpp"
#include "EnumTypeDefinition.hpp"
#include "InvalidEnumValueException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* NumberFromEnumFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	this->CheckArguments(location, 1, arguments);

	if (arguments[0]->IsStaticObject())
	{
		throw IncorrectTypeException(GGE2101_IncorrectNativeFunctionArgumentType, location, GG_STR("Enum"), GG_STR("Type"));
	}
	else if (!arguments[0]->GetType()->IsEnumType())
	{
		throw IncorrectTypeException(GGE2101_IncorrectNativeFunctionArgumentType, location, GG_STR("Enum"), arguments[0]->GetType()->GetName());
	}

	NumberObject* numberValue = dynamic_cast<NumberObject*>(arguments[0]);

	return numberTypeDefinition->CreateInstance(vm, numberValue->GetValue());
}