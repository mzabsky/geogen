#include "HeightMapNoiseFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../genlib/NoiseLayersFactory.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "ArrayTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightMapNoiseRenderingStep.hpp"
#include "ParseNoiseInput.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;
using namespace geogen::random;
using namespace geogen::genlib;

ManagedObject* HeightMapNoiseFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	ArrayTypeDefinition const* arrayTypeDefinition = dynamic_cast<ArrayTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Array")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(arrayTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	
	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 0);

	NoiseLayers layers = ParseNoiseInput(vm, location, arguments);

	RandomSeed argumentSeed = arguments.size() > 1 ? (RandomSeed)dynamic_cast<NumberObject*>(arguments[1])->GetValue() : 0;
	RandomSeed compositeSeed = CombineSeeds(argumentSeed, vm->GetArguments().GetRandomSeed());

	ManagedObject* returnObject = dynamic_cast<HeightMapTypeDefinition const*>(instance->GetType())->CreateInstance(vm);
	
	// First create an empty height map
	unsigned objectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightMapFlatRenderingStep(location, vector<unsigned>(), objectSlot, 0);
	vm->AddRenderingStep(location, renderingStep);

	// Then generate the noise layer by layer
	vector<unsigned> argumentSlots;
	argumentSlots.push_back(objectSlot);

	unsigned i = 0;
	for (NoiseLayers::const_iterator it = layers.begin(); it != layers.end(); it++)
	{
		RenderingStep* renderingStep = new HeightMapNoiseRenderingStep(location, argumentSlots, objectSlot, it->first, it->second, compositeSeed, i);
		vm->AddRenderingStep(location, renderingStep);
		i++;
	}

	return returnObject;
}