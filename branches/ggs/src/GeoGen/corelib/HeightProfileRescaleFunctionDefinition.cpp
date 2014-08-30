#include "HeightProfileRescaleFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightProfileRescaleRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfileRescaleFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Scale scale = dynamic_cast<NumberObject*>(arguments[0])->GetValue();

	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightProfileRescaleRenderingStep(location, argumentSlots, returnObjectSlot, scale);
	vm->GetRenderingSequence().AddStep(renderingStep);

	return instance;
}