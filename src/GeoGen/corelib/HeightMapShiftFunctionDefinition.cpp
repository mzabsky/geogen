#include "HeightMapShiftFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "SizeOverflowException.hpp"
#include "HeightMapShiftRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "DirectionTypeDefinition.hpp"
#include "../Direction.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapShiftFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	HeightProfileTypeDefinition const* heightProfileTypeDefinition = dynamic_cast<HeightProfileTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("HeightProfile")));
	DirectionTypeDefinition const* directionTypeDefinition = dynamic_cast<DirectionTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Direction")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(heightProfileTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(directionTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Number numberMaxDistance = ((NumberObject*)arguments[1])->GetValue();
	Size1D maxDistance;
	if (!TryNumberToSize(numberMaxDistance, maxDistance))
	{
		throw SizeOverflowException(location);
	}

	Direction direction = (Direction)NumberToInt(((NumberObject*)arguments[2])->GetValue());
	
	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapShiftRenderingStep(location, argumentSlots, returnObjectSlot, maxDistance, direction);
	vm->AddRenderingStep(location, renderingStep);

	return instance;
}