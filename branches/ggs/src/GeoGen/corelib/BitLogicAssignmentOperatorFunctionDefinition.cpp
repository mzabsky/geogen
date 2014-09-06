#include "BitLogicAssignmentOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../corelib/BooleanTypeDefinition.hpp"
#include "ReferenceTypeDefinition.hpp"
#include "ReferenceObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "DivisionByZeroException.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/NumberOverflowException.hpp"
#include "../runtime/NumberUnderflowException.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;

BitLogicAssignmentOperatorFunctionDefinition* BitLogicAssignmentOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case AND: return new BitLogicAssignmentOperatorFunctionDefinition(GG_STR("&="), op);
	case OR: return new BitLogicAssignmentOperatorFunctionDefinition(GG_STR("|="), op);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* BitLogicAssignmentOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	ReferenceTypeDefinition const* referenceTypeDefinition = dynamic_cast<ReferenceTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("<Reference>")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	bool returnsNumber = true;
	if (arguments[1]->GetType() == booleanTypeDefinition)
	{
		returnsNumber = false;
	}

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(referenceTypeDefinition);
	expectedTypes.push_back(returnsNumber ? (TypeDefinition const*)numberTypeDefinition : (TypeDefinition const*)booleanTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	ReferenceObject* reference = dynamic_cast<ReferenceObject*>(arguments[0]);

	ManagedObjectHolder referencedObjectHolder;
	ManagedObject* referencedObject = reference->GetReferencedObject(location, vm);
	this->CheckArgument(vm, location, returnsNumber ? (TypeDefinition const*)numberTypeDefinition : (TypeDefinition const*)booleanTypeDefinition, referencedObject, referencedObjectHolder);

	Number input = returnsNumber ? dynamic_cast<NumberObject*>(referencedObject)->GetValue() : (dynamic_cast<BooleanObject*>(referencedObject)->GetValue() ? 1 : 0);
	Number argument = returnsNumber ? dynamic_cast<NumberObject*>(arguments[1])->GetValue() : (dynamic_cast<BooleanObject*>(arguments[1])->GetValue() ? 1 : 0);

	RuntimeMathCheckInit();

	Number result;

	switch (this->op)
	{
	case AND: result = int(input) & int(argument); break;
	case OR: result = int(input) | int(argument); break;
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}

	RuntimeMathCheck(location);

	ManagedObject* returnObject;
	if (returnsNumber)
	{
		returnObject = numberTypeDefinition->CreateInstance(vm, result);
	}
	else
	{
		returnObject = booleanTypeDefinition->CreateInstance(vm, result > 0);
	}

	reference->SetReferencedObject(location, vm, returnObject);

	return returnObject;
}