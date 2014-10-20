#include "HeightMapRescaleFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "InvalidScaleException.hpp"
#include "HeightMapRescaleRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapRescaleFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 1);

	Number numberHorizontalScale = ((NumberObject*)arguments[0])->GetValue();
	Scale horizontalScale;
	if (!TryNumberToScale(numberHorizontalScale, horizontalScale))
	{
		throw InvalidScaleException(location);
	}

	Number numberVerticalScale = arguments.size() > 1 ? ((NumberObject*)arguments[1])->GetValue() : numberHorizontalScale;
	Scale verticalScale;
	if (!TryNumberToScale(numberVerticalScale, verticalScale))
	{
		throw InvalidScaleException(location);
	}

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapRescaleRenderingStep(location, argumentSlots, returnObjectSlot, horizontalScale, verticalScale);
	vm->AddRenderingStep(location, renderingStep);

	return instance;
}