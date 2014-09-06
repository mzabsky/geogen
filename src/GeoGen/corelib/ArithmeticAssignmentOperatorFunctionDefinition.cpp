#include "ArithmeticAssignmentOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../runtime/ReferenceTypeDefinition.hpp"
#include "../runtime/ReferenceObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "DivisionByZeroException.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/NumberOverflowException.hpp"
#include "../runtime/NumberUnderflowException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ArithmeticAssignmentOperatorFunctionDefinition* ArithmeticAssignmentOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case MULTIPLICATION: return new ArithmeticAssignmentOperatorFunctionDefinition(GG_STR("*="), op);
	case DIVISION: return new ArithmeticAssignmentOperatorFunctionDefinition(GG_STR("/="), op);
	case MODULO: return new ArithmeticAssignmentOperatorFunctionDefinition(GG_STR("%="), op);
	case ADDITION: return new ArithmeticAssignmentOperatorFunctionDefinition(GG_STR("+="), op);
	case SUBTRACTION: return new ArithmeticAssignmentOperatorFunctionDefinition(GG_STR("-="), op);
	case BIT_SHIFT_LEFT: return new ArithmeticAssignmentOperatorFunctionDefinition(GG_STR("<<="), op);
	case BIT_SHIFT_RIGHT: return new ArithmeticAssignmentOperatorFunctionDefinition(GG_STR(">>="), op);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* ArithmeticAssignmentOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	ReferenceTypeDefinition const* referenceTypeDefinition = dynamic_cast<ReferenceTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("<Reference>")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(referenceTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	ReferenceObject* reference = dynamic_cast<ReferenceObject*>(arguments[0]);

	ManagedObjectHolder referencedObjectHolder;
	ManagedObject* referencedObject = reference->GetReferencedObject(location, vm);
	this->CheckArgument(vm, location, numberTypeDefinition, referencedObject, referencedObjectHolder);

	Number input = dynamic_cast<NumberObject*>(referencedObject)->GetValue();
	Number argument = dynamic_cast<NumberObject*>(arguments[1])->GetValue();

	RuntimeMathCheckInit();

	Number result;

	switch (this->op)
	{
	case MULTIPLICATION: result = input * argument; break;
	case DIVISION: result = input / argument; break;
	case MODULO: result = int(input) % int(argument); break;
	case ADDITION: result = input + argument; break;
	case SUBTRACTION: result = input - argument; break;
	case BIT_SHIFT_LEFT: result = int(input) << int(argument); break;
	case BIT_SHIFT_RIGHT: result = int(input) >> int(argument); break;
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}

	RuntimeMathCheck(location);


	ManagedObject* returnObject = numberTypeDefinition->CreateInstance(vm, result);

	reference->SetReferencedObject(location, vm, returnObject);

	return returnObject;
}