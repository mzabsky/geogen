#include "HeightProfileNoiseFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "ArrayTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightProfileFlatRenderingStep.hpp"
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

	// First create an empty height map
	unsigned objectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* flatRenderingStep = new HeightProfileFlatRenderingStep(location, vector<unsigned>(), objectSlot, 0);
	vm->GetRenderingSequence().AddStep(flatRenderingStep);

	// Then generate the noise layer by layer
	vector<unsigned> argumentSlots;
	argumentSlots.push_back(objectSlot);

	unsigned i = 0;
	for (NoiseLayers::const_iterator it = layers.begin(); it != layers.end(); it++)
	{
		RenderingStep* renderingStep = new HeightProfileNoiseRenderingStep(location, argumentSlots, objectSlot, it->first, it->second, 0, i);
		vm->GetRenderingSequence().AddStep(renderingStep);
		i++;
	}

	return returnObject;
}