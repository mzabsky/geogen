#include "NumberFromCoordinateFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* NumberFromCoordinateFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(coordinateTypeDefinition);

	this->CheckArguments(location, expectedTypes, arguments);

	CoordinateObject* coordinateValue = dynamic_cast<CoordinateObject*>(arguments[0]);

	return numberTypeDefinition->CreateInstance(vm, coordinateValue->GetValue());
}