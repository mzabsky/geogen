#include "PointCreateFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "PointTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "CoordinateObject.hpp"
#include "RelativeCoordinateInInfiniteDimensionException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* PointCreateFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));
	PointTypeDefinition const* coordinateType = dynamic_cast<PointTypeDefinition const*>(this->GetOwningTypeDefinition());

	vector<TypeDefinition const*> expectedTypes;	
	expectedTypes.push_back(coordinateTypeDefinition);	
	expectedTypes.push_back(coordinateTypeDefinition);

	this->CheckArguments(location, expectedTypes, arguments);

	CoordinateObject* coordX = dynamic_cast<CoordinateObject*>(arguments[0]);
	CoordinateObject* coordY = dynamic_cast<CoordinateObject*>(arguments[1]);
	
	if ((coordX->IsRelative() && vm->GetArguments().GetMapWidth() == MAP_SIZE_INFINITE) || (coordY->IsRelative() && vm->GetArguments().GetMapHeight() == MAP_SIZE_INFINITE))
	{
		throw RelativeCoordinateInInfiniteDimensionException(location);
	}

	return coordinateType->CreateInstance(vm, coordX->GetValue(), coordY->GetValue(), coordX->IsRelative(), coordY->IsRelative());
}