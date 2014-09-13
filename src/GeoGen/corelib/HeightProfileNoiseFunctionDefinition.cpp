#include "HeightProfileNoiseFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "ArrayTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightProfileNoiseRenderingStep.hpp"
#include "ParseNoiseInput.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;
using namespace geogen::genlib;

ManagedObject* HeightProfileNoiseFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	ArrayTypeDefinition const* arrayTypeDefinition = dynamic_cast<ArrayTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Array")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(arrayTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 0);

	NoiseLayers layers = ParseNoiseInput(vm, location, arguments);

	ManagedObject* returnObject = dynamic_cast<HeightProfileTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightProfileNoiseRenderingStep(location, argumentSlots, returnObjectSlot, layers, vm->GetArguments().GetRandomSeed());
	vm->GetRenderingSequence().AddStep(renderingStep);

	return returnObject;
}