#include "HeightMapConvexityMapFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapConvexityMapRenderingStep.hpp"
#include "SizeOverflowException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapConvexityMapFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Number numberRadius = ((NumberObject*)arguments[0])->GetValue();
	Size1D radius;
	if (!TryNumberToSize(numberRadius, radius))
	{
		throw SizeOverflowException(location);
	}

	ManagedObject* returnObject = dynamic_cast<HeightMapTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightMapConvexityMapRenderingStep(location, argumentSlots, returnObjectSlot, radius);
	vm->GetRenderingSequence().AddStep(renderingStep);

	return returnObject;
}