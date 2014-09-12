#include "HeightMapNoiseFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/IncorrectTypeException.hpp"
#include "../genlib/NoiseLayersFactory.hpp"
#include "NumberTypeDefinition.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "ArrayTypeDefinition.hpp"
#include "HeightMapNoiseRenderingStep.hpp"
#include "HeightOverflowException.hpp"

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
	
	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 0);

	NoiseLayers layers;

	if (arguments.size() > 0)
	{		
	}
	else 
	{
		layers = NoiseLayersFactory::CreateDefaultLayers();
	}

	/*Number numberHeight = arguments.size() > 0 ? ((NumberObject*)arguments[0])->GetValue() : 0;
	Height height;
	if (!TryNumberToHeight(numberHeight, height))
	{
		throw HeightOverflowException(location);
	}*/

	ManagedObject* returnObject = dynamic_cast<HeightMapTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightMapNoiseRenderingStep(location, argumentSlots, returnObjectSlot, layers, 0);
	vm->GetRenderingSequence().AddStep(renderingStep);

	return returnObject;
}