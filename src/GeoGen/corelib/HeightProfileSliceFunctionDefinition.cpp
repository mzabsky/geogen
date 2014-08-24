#include "HeightProfileSliceFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "NumberTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "CoordinateObject.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "DirectionTypeDefinition.hpp"
#include "../Direction.hpp"
#include "HeightProfileSliceRenderingStep.hpp"
#include "UnknownRelativeCoordinateDirectionException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfileSliceFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	HeightMapTypeDefinition const* heightMapTypeDefinition = dynamic_cast<HeightMapTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("HeightMap")));
	DirectionTypeDefinition const* directionTypeDefinition = dynamic_cast<DirectionTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Direction")));
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(heightMapTypeDefinition);
	expectedTypes.push_back(directionTypeDefinition);
	expectedTypes.push_back(coordinateTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Direction direction = (Direction)NumberToInt(dynamic_cast<NumberObject*>(arguments[1])->GetValue());
	Coordinate coordinate = dynamic_cast<CoordinateObject*>(arguments[2])->GetAbsoluteCoordinate(vm, location, OtherDirection(direction));
	
	ManagedObject* returnObject = dynamic_cast<HeightProfileTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	//argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject));
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightProfileSliceRenderingStep(location, argumentSlots, returnObjectSlot, direction, coordinate);
	vm->GetRenderingSequence().AddStep(renderingStep);

	return returnObject;
}