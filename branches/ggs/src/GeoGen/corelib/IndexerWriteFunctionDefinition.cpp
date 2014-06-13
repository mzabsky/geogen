#include "IndexerWriteFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "..\runtime\IncorrectTypeException.hpp"
#include "NumberTypeDefinition.hpp"
#include "BooleanTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* IndexerWriteFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	this->CheckArguments(location, 1, arguments);

	if (arguments[0]->IsStaticObject())
	{
		throw IncorrectTypeException(GGE2102_IncorrectOperandType, location, numberTypeDefinition->GetName(), GG_STR("Static"));
	}

	ManagedObject* returnObject;
	/*if (arguments[0]->GetType() == numberTypeDefinition)
	{
	int value = (int)dynamic_cast<NumberObject*>(arguments[0])->GetValue();
	returnObject = numberTypeDefinition->CreateInstance(!(int)value);
	}
	else */if (arguments[0]->GetType() == booleanTypeDefinition){
		bool value = dynamic_cast<BooleanObject*>(arguments[0])->GetValue();
		returnObject = booleanTypeDefinition->CreateInstance(vm, !value);
	}
	else
	{
		throw IncorrectTypeException(GGE2102_IncorrectOperandType, location, booleanTypeDefinition->GetName(), arguments[0]->GetType()->GetName());
	}

	return returnObject;
}