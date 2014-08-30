#include "RandomFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "MaxLessThanMinException.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* RandomFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedParameters;
	if (arguments.size() > 0)
	{
		expectedParameters.push_back(numberTypeDefinition);
		expectedParameters.push_back(numberTypeDefinition);
	}
	
	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedParameters, arguments);

	Number n;
	if (arguments.size() > 0)
	{
		Number minValue = dynamic_cast<NumberObject*>(arguments[0])->GetValue();
		Number maxValue = dynamic_cast<NumberObject*>(arguments[1])->GetValue();

		if (minValue > maxValue)
		{
			throw MaxLessThanMinException(location);
		}

		n = vm->GetCommonRandomSequence().NextDouble() * (maxValue - minValue) + minValue;
	}
	else 
	{
		n = vm->GetCommonRandomSequence().NextDouble();
	}

	return numberTypeDefinition->CreateInstance(vm, n);
}