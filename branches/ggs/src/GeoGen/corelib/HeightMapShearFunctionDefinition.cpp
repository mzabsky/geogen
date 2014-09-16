#include "HeightMapShearFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapTransformRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"
#include "DirectionTypeDefinition.hpp"
#include "../Direction.hpp"
#include "InvalidTransformationMatrixException.hpp"
#include "../genlib/TransformationMatrix.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace renderer;
using namespace genlib;

ManagedObject* HeightMapShearFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	DirectionTypeDefinition const* directionTypeDefinition = dynamic_cast<DirectionTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Direction")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(directionTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Number shearFactor = ((NumberObject*)arguments[0])->GetValue();
	Direction direction = (Direction)(int)((NumberObject*)arguments[1])->GetValue();

	TransformationMatrix matrix;
	if (direction == DIRECTION_HORIZONTAL)
	{
		matrix.A11 = 1;
		matrix.A12 = shearFactor;
		matrix.A21 = 0;
		matrix.A22 = 1;
	}
	else if (direction == DIRECTION_VERTICAL)
	{

		matrix.A11 = 1;
		matrix.A12 = 0;
		matrix.A21 = shearFactor;
		matrix.A22 = 1;
	}
	else 
	{
		throw InternalErrorException(GG_STR("Invalid direction."));
	}

	if (!matrix.IsInvertible())
	{
		throw InvalidTransformationMatrixException(location);
	}

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapTransformRenderingStep(location, argumentSlots, returnObjectSlot, matrix);
	vm->GetRenderingSequence().AddStep(renderingStep);

	return instance;
}