#include "RelativeCoordinateOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "CoordinateObject.hpp"
#include "CoordinateTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* RelativeCoordinateOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition("Coordinate"));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(coordinateTypeDefinition);

	this->CheckArguments(location, expectedTypes, arguments);

	CoordinateObject* input = dynamic_cast<CoordinateObject*>(arguments[0]);
	if (input->IsRelative())
	{
		return input;
	}
	else 
	{
		return coordinateTypeDefinition->CreateInstance(vm, input->GetValue(), true);
	}
}