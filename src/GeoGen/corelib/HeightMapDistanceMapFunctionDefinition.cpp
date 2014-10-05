#include "HeightMapDistanceMapFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapDistanceMapRenderingStep.hpp"
#include "SizeOverflowException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapDistanceMapFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	HeightMapTypeDefinition const* heightMapTypeDefinition = dynamic_cast<HeightMapTypeDefinition const*>(instance->GetType());
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(heightMapTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Number numberDistance = ((NumberObject*)arguments[1])->GetValue();
	Size1D distance;
	if (!TryNumberToSize(numberDistance, distance))
	{
		throw SizeOverflowException(location);
	}

	ManagedObject* returnObject = heightMapTypeDefinition->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightMapDistanceMapRenderingStep(location, argumentSlots, returnObjectSlot, distance);
	vm->GetRenderingSequence().AddStep(renderingStep);

	return returnObject;
}