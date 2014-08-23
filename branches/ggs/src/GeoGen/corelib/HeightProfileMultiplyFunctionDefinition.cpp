#include "HeightProfileMultiplyFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightProfileFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightProfileMultiplyRenderingStep.hpp"
#include "HeightProfileMultiplyProfileRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfileMultiplyFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
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
		RenderingStep* renderingStep = new HeightProfileMultiplyRenderingStep(location, argumentSlots, returnObjectSlot, height);
		vm->GetRenderingSequence().AddStep(renderingStep);
	}
	else
	{
		argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));

		unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
		RenderingStep* renderingStep = new HeightProfileMultiplyProfileRenderingStep(location, argumentSlots, returnObjectSlot);
		vm->GetRenderingSequence().AddStep(renderingStep);
	}

	return instance;
}