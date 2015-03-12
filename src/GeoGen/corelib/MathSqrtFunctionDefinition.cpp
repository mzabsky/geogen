#include "MathSqrtFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../InternalErrorException.hpp"
#include "NumberTypeDefinition.hpp"
#include "MathDefinitionRangeException.hpp"
#include "../runtime/NumberUnderflowException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* MathSqrtFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedParameters;
	expectedParameters.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedParameters, arguments);

	Number input = dynamic_cast<NumberObject*>(arguments[0])->GetValue();

	if (input < 0) throw MathDefinitionRangeException(GGE2307_OutsideSqrtFunctionDefinitionRange, location, this->GetName(), input);

	Number result = sqrt(input);

	if (input != 0 && result == 0)
	{
		throw NumberUnderflowException(location);
	}

	return numberTypeDefinition->CreateInstance(vm, result);
}