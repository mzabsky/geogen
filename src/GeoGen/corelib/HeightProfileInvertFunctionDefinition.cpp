#include "HeightProfileInvertFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightProfileInvertRenderingStep.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfileInvertFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	vector<TypeDefinition const*> expectedTypes;

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightProfileInvertRenderingStep(location, argumentSlots, returnObjectSlot);
	vm->GetRenderingSequence().AddStep(renderingStep);

	return instance;
}