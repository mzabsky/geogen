#include "IncrementDecrementOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../runtime/ReferenceTypeDefinition.hpp"
#include "../runtime/ReferenceObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "DivisionByZeroException.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

IncrementDecrementOperatorFunctionDefinition* IncrementDecrementOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case PLUS_PLUS_PRE: return new IncrementDecrementOperatorFunctionDefinition(GG_STR("++pre"), op);
	case PLUS_PLUS_POST: return new IncrementDecrementOperatorFunctionDefinition(GG_STR("++post"), op);
	case MINUS_MINUS_PRE: return new IncrementDecrementOperatorFunctionDefinition(GG_STR("--pre"), op);
	case MINUS_MINUS_POST: return new IncrementDecrementOperatorFunctionDefinition(GG_STR("--post"), op);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* IncrementDecrementOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	ReferenceTypeDefinition const* referenceTypeDefinition = dynamic_cast<ReferenceTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("<Reference>")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(referenceTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	ReferenceObject* reference = dynamic_cast<ReferenceObject*>(arguments[0]);

	ManagedObjectHolder referencedObjectHolder;
	this->CheckArgument(vm, location, numberTypeDefinition, reference->GetReferencedObject(location, vm), referencedObjectHolder);

	Number number = dynamic_cast<NumberObject*>(reference->GetReferencedObject(location, vm))->GetValue();
	Number resultNumber;
	Number returnNumber;

	switch (op)
	{
	case PLUS_PLUS_PRE: resultNumber = number + 1; returnNumber = resultNumber; break;
	case PLUS_PLUS_POST: resultNumber = number + 1; returnNumber = number; break;
	case MINUS_MINUS_PRE: resultNumber = number - 1; returnNumber = resultNumber; break;
	case MINUS_MINUS_POST: resultNumber = number - 1; returnNumber = number; break;
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}

	reference->SetReferencedObject(location, vm, numberTypeDefinition->CreateInstance(vm, resultNumber));

	ManagedObject* returnObject = numberTypeDefinition->CreateInstance(vm, returnNumber);
	return returnObject;
}