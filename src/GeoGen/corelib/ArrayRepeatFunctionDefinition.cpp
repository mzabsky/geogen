#include "ArrayRepeatFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* ArrayRepeatFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(arguments.size() > 0 ? arguments[0]->GetType() : numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	int number = NumberToInt(dynamic_cast<NumberObject*>(arguments[1])->GetValue());

	ArrayTypeDefinition const* arrayTypeDefinition = dynamic_cast<ArrayTypeDefinition const*>(instance->GetType());

	ArrayObject* returnObject = dynamic_cast<ArrayObject*>(arrayTypeDefinition->CreateInstance(vm));

	for (int i = 0; i < number; i++)
	{
		returnObject->PushBack(vm, location, arguments[0]);
	}

	return returnObject;
}