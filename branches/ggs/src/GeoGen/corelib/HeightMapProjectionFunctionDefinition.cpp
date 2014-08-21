#include "HeightMapProjectionFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "../Orientation.hpp"
#include "OrientationTypeDefinition.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapProjectionRenderingStep.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapProjectionFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	HeightProfileTypeDefinition const* heightProfileTypeDefinition = dynamic_cast<HeightProfileTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("HeightProfile")));
	OrientationTypeDefinition const* orientationTypeDefinition = dynamic_cast<OrientationTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Orientation")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(heightProfileTypeDefinition);
	expectedTypes.push_back(orientationTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Orientation orientation = (Orientation)NumberToInt(dynamic_cast<NumberObject*>(arguments[1])->GetValue());

	ManagedObject* returnObject = dynamic_cast<HeightMapTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightMapProjectionRenderingStep(location, argumentSlots, returnObjectSlot, orientation);
	vm->GetRenderingSequence().AddStep(renderingStep);

	return returnObject;
}