#include "MathSqrtFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/IncorrectTypeException.hpp"
#include "NumberTypeDefinition.hpp"
#include "MathDefinitionRangeException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* MathSqrtFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedParameters;
	expectedParameters.push_back(numberTypeDefinition);

	this->CheckArguments(location, expectedParameters, arguments);

	Number input = dynamic_cast<NumberObject*>(arguments[0])->GetValue();

	RuntimeMathCheckInit();

	if (input < 0) throw MathDefinitionRangeException(GGE2307_OutsideSqrtFunctionDefinitionRange, location, this->GetName(), input);

	RuntimeMathCheck(location);
	
	Number result = sqrt(input);

	return numberTypeDefinition->CreateInstance(vm, result);
}