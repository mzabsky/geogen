#include "HeightProfileFromArrayFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/IncorrectTypeException.hpp"
#include "NumberTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "ArrayTypeDefinition.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightProfileFromArrayRenderingStep.hpp"
#include "DirectionTypeDefinition.hpp"
#include "HeightOverflowException.hpp"
#include "ArrayObject.hpp"
#include "CoordinateObject.hpp"
#include "UnknownRelativeCoordinateDirectionException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfileFromArrayFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));
	ArrayTypeDefinition const* arrayTypeDefinition = dynamic_cast<ArrayTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Array")));
	DirectionTypeDefinition const* directionTypeDefinition = dynamic_cast<DirectionTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Direction")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(arrayTypeDefinition);
	expectedTypes.push_back(directionTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 1);

	ArrayObject* arrayObject = (ArrayObject*)arguments[0];

	bool hasDirection = false;
	Direction direction = DIRECTION_HORIZONTAL;
	if (arguments.size() > 1)
	{
		direction = (Direction)NumberToInt(dynamic_cast<NumberObject*>(arguments[1])->GetValue());
		hasDirection = true;
	}

	map<Coordinate, Height> heights;
	for (ArrayObject::HashMap::iterator it = arrayObject->MBegin(); it != arrayObject->MEnd(); it++)
	{
		Coordinate key;
		if (it->first->GetType() == numberTypeDefinition)
		{
			key = dynamic_cast<NumberObject*>(it->first)->GetValue();
		}
		else if (it->first->GetType() == coordinateTypeDefinition)
		{
			CoordinateObject* coordinateObject = dynamic_cast<CoordinateObject*>(it->first);
			if (coordinateObject->IsRelative() && !hasDirection)
			{
				throw UnknownRelativeCoordinateDirectionException(location);
			}

			// TODO: Range check?
			key = coordinateObject->GetAbsoluteCoordinate(vm, location, direction);
		}
		else IncorrectTypeException(GGE2706_IncorrectHeightProfileKeyType, location, GG_STR("Number"), it->second->GetType()->GetName());

		Height value;
		if (it->second->GetType() == numberTypeDefinition)
		{
			if (!TryNumberToHeight(dynamic_cast<NumberObject*>(it->second)->GetValue(), value))
			{
				throw HeightOverflowException(location);
			}
		}
		else IncorrectTypeException(GGE2707_IncorrectHeightProfileHeightType, location, GG_STR("Number"), it->second->GetType()->GetName());

		heights.insert(pair<Coordinate, Height>(key, value));
	}

	ManagedObject* returnObject = dynamic_cast<HeightProfileTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightProfileFromArrayRenderingStep(location, argumentSlots, returnObjectSlot, heights);
	vm->GetRenderingSequence().AddStep(renderingStep);

	return returnObject;
}