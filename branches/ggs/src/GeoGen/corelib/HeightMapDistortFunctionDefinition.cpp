#include "HeightMapDistortFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapDistortRenderingStep.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightMapNoiseRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapDistortFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	// TODO: size check
	Size1D maxDistance = dynamic_cast<NumberObject*>(arguments[0])->GetValue();
	Size1D perturbanceSize = dynamic_cast<NumberObject*>(arguments[1])->GetValue();

	// TODO: Seed

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);

	ManagedObject* horizontalNoiseMap = dynamic_cast<HeightMapTypeDefinition const*>(instance->GetType())->CreateInstance(vm);
	ManagedObject* verticalNoiseMap = dynamic_cast<HeightMapTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	unsigned horizontalNoiseSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(horizontalNoiseMap);
	unsigned verticalNoiseSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(verticalNoiseMap);

	// Create flat horizontal map
	{		
		vector<unsigned> argumentSlots;
		RenderingStep* renderingStep = new HeightMapFlatRenderingStep(location, argumentSlots, horizontalNoiseSlot, 0);
		vm->GetRenderingSequence().AddStep(renderingStep);
	}

	// Horizontal map noise
	{
		vector<unsigned> argumentSlots;
		argumentSlots.push_back(horizontalNoiseSlot);
		RenderingStep* renderingStep = new HeightMapNoiseRenderingStep(location, argumentSlots, horizontalNoiseSlot, perturbanceSize, HEIGHT_MAX, 0, 0);
		vm->GetRenderingSequence().AddStep(renderingStep);
	}

	// Create flat vertical map
	{
		vector<unsigned> argumentSlots;
		RenderingStep* renderingStep = new HeightMapFlatRenderingStep(location, argumentSlots, verticalNoiseSlot, 0);
		vm->GetRenderingSequence().AddStep(renderingStep);
	}

	// Vertical map noise
	{
		vector<unsigned> argumentSlots;
		argumentSlots.push_back(verticalNoiseSlot);
		RenderingStep* renderingStep = new HeightMapNoiseRenderingStep(location, argumentSlots, verticalNoiseSlot, perturbanceSize, HEIGHT_MAX, 1, 0);
		vm->GetRenderingSequence().AddStep(renderingStep);
	}

	// Distort call 
	{
		vector<unsigned> argumentSlots;		
		argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
		argumentSlots.push_back(horizontalNoiseSlot);
		argumentSlots.push_back(verticalNoiseSlot);
		RenderingStep* renderingStep = new HeightMapDistortRenderingStep(location, argumentSlots, returnObjectSlot, maxDistance);
		vm->GetRenderingSequence().AddStep(renderingStep);
	}

	return instance;
}