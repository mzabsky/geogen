#include "EnumFromNumberFunctionDefinition.hpp"
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

ManagedObject* EnumFromNumberFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	EnumTypeDefinition const* enumType = dynamic_cast<EnumTypeDefinition const*>(this->GetOwningTypeDefinition());

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Number input = dynamic_cast<NumberObject*>(arguments[0])->GetValue();	

	if (!enumType->IsValueIntDefined(NumberToInt(input)))
	{
		throw InvalidEnumValueException(location, NumberToInt(input), enumType->GetName());
	}

	return enumType->CreateInstance(vm, NumberToInt(input));
}