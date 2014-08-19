#include "HeightMapCropHeightsFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapCropHeightsRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"
#include "MaxLessThanMinException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapCropHeightsFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 2);

	Number numberMin = ((NumberObject*)arguments[0])->GetValue();
	Height min;
	if (!TryNumberToHeight(numberMin, min))
	{
		throw HeightOverflowException(location);
	}

	Number numberMax = ((NumberObject*)arguments[1])->GetValue();
	Height max;
	if (!TryNumberToHeight(numberMax, max))
	{
		throw HeightOverflowException(location);
	}

	Number numberReplace = arguments.size() > 2 ? ((NumberObject*)arguments[2])->GetValue() : 0;
	Height replace;
	if (!TryNumberToHeight(numberReplace, replace))
	{
		throw HeightOverflowException(location);
	}

	if (min > max)
	{
		throw MaxLessThanMinException(location);
	}

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapCropHeightsRenderingStep(location, argumentSlots, returnObjectSlot, min, max, replace);
	vm->GetRenderingSequence().AddStep(renderingStep);

	return instance;
}