#include "HeightProfileCropHeightsFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightProfileCropHeightsRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"
#include "HeightOverflowException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfileCropHeightsFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 2);

	Number numberMinHeight = ((NumberObject*)arguments[0])->GetValue();
	Height minHeight;
	if (!TryNumberToHeight(numberMinHeight, minHeight))
	{
		throw HeightOverflowException(location);
	}

	Number numberMaxHeight = ((NumberObject*)arguments[1])->GetValue();
	Height maxHeight;
	if (!TryNumberToHeight(numberMaxHeight, maxHeight))
	{
		throw HeightOverflowException(location);
	}

	Number numberReplace = arguments.size() > 2 ? ((NumberObject*)arguments[2])->GetValue() : 0;
	Height replace;
	if (!TryNumberToHeight(numberReplace, replace))
	{
		throw HeightOverflowException(location);
	}

	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightProfileCropHeightsRenderingStep(location, argumentSlots, returnObjectSlot, minHeight, maxHeight, replace);
	vm->AddRenderingStep(location, renderingStep);

	return instance;
}