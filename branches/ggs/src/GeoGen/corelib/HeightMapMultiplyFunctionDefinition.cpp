#include "HeightMapMultiplyFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapMultiplyRenderingStep.hpp"
#include "HeightMapMultiplyMapRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapMultiplyFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;

	bool isHeightMode;
	if (arguments.size() > 0 && arguments[0]->GetType() == this->GetOwningTypeDefinition())
	{
		isHeightMode = false;
		expectedTypes.push_back(this->GetOwningTypeDefinition());
	}
	else
	{
		isHeightMode = true;
		expectedTypes.push_back(numberTypeDefinition);
	}

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));

	if (isHeightMode)
	{
		Number numberHeight = ((NumberObject*)arguments[0])->GetValue();
		Height height;
		if (!TryNumberToHeight(numberHeight, height))
		{
			throw HeightOverflowException(location);
		}

		unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
		RenderingStep* renderingStep = new HeightMapMultiplyRenderingStep(location, argumentSlots, returnObjectSlot, height);
		vm->GetRenderingSequence().AddStep(renderingStep);
	}
	else
	{
		argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));

		unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
		RenderingStep* renderingStep = new HeightMapMultiplyMapRenderingStep(location, argumentSlots, returnObjectSlot);
		vm->GetRenderingSequence().AddStep(renderingStep);
	}

	return instance;
}