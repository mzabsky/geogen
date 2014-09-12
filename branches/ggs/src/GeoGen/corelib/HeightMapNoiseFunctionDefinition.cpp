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
#include "ArrayObject.hpp"
#include "HeightMapNoiseRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "NonpositiveNoiseLayerWaveLengthException.hpp"
#include "NonpositiveNoiseLayerAmplitudeException.hpp"

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
		ArrayObject* array = dynamic_cast<ArrayObject*>(arguments[0]);

		for (ArrayObject::iterator it = array->Begin(); it != array->End(); it++)
		{			
			ManagedObject* key = *it;
			ManagedObject* value = array->Get(vm, location, key);
			if (key->GetType() != numberTypeDefinition)
			{
				throw IncorrectTypeException(GGE2708_IncorrectNoiseLayerWaveLengthType, location, GG_STR("Number"), (*it)->GetType()->GetName());
			}

			if (value->GetType() != numberTypeDefinition)
			{
				throw IncorrectTypeException(GGE2709_IncorrectNoiseLayerAmplitudeType, location, GG_STR("Number"), (*it)->GetType()->GetName());
			}

			Number waveLengthNumber = dynamic_cast<NumberObject*>(key)->GetValue();
			Number amplitudeNumber = dynamic_cast<NumberObject*>(value)->GetValue();

			if (waveLengthNumber <= 0)
			{
				throw NonpositiveNoiseLayerWaveLengthException(location);
			}

			if (amplitudeNumber <= 0)
			{
				throw NonpositiveNoiseLayerAmplitudeException(location);
			}

			Size1D waveLength = (Size1D)waveLengthNumber;

			Height amplitude;
			if (!TryNumberToHeight(amplitudeNumber, amplitude))
			{
				throw HeightOverflowException(location);
			}
			
			layers[waveLength] = amplitude;
		}
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