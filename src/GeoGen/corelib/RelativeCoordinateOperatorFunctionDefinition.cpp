#include "RelativeCoordinateOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "CoordinateObject.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "PointObject.hpp"
#include "PointTypeDefinition.hpp"
#include "RelativeCoordinateInInfiniteDimensionException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* RelativeCoordinateOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));
	PointTypeDefinition const* pointTypeDefinition = dynamic_cast<PointTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Point")));

	vector<TypeDefinition const*> expectedTypes;

	if (arguments.size() > 0 && arguments[0]->GetType() == pointTypeDefinition)
	{
		expectedTypes.push_back(pointTypeDefinition);

		vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

		if (vm->GetArguments().GetMapWidth() == MAP_SIZE_INFINITE && vm->GetArguments().GetMapHeight() == MAP_SIZE_INFINITE)
		{
			throw RelativeCoordinateInInfiniteDimensionException(location);
		}

		PointObject* input = dynamic_cast<PointObject*>(arguments[0]);
		if (input->IsXRelative() && input->IsYRelative())
		{
			return input;
		}
		else
		{
			return pointTypeDefinition->CreateInstance(vm, input->GetX(), input->GetY(), true, true);
		}
	}
	else 
	{
		expectedTypes.push_back(coordinateTypeDefinition);

		vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

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
}