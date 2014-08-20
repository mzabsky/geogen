#include "HeightProfileAddFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightProfileFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightProfileAddRenderingStep.hpp"
#include "HeightProfileAddProfileRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfileAddFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
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

	expectedTypes.push_back(this->GetOwningTypeDefinition()); // mask

	bool hasMask = arguments.size() > 1;

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 1);

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));

	if (isHeightMode)
	{
		if (hasMask)
		{
			argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[1]));
		}

		Number numberHeight = ((NumberObject*)arguments[0])->GetValue();
		Height height;
		if (!TryNumberToHeight(numberHeight, height))
		{
			throw HeightOverflowException(location);
		}

		unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
		RenderingStep* renderingStep = new HeightProfileAddRenderingStep(location, argumentSlots, returnObjectSlot, height);
		vm->GetRenderingSequence().AddStep(renderingStep);
	}
	else
	{
		argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));

		if (hasMask)
		{
			argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[1]));
		}

		unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
		RenderingStep* renderingStep = new HeightProfileAddProfileRenderingStep(location, argumentSlots, returnObjectSlot);
		vm->GetRenderingSequence().AddStep(renderingStep);
	}

	return instance;
}