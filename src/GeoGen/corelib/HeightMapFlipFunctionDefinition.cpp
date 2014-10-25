#include "HeightMapFlipFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapTransformRenderingStep.hpp"
#include "DirectionTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "InvalidTransformationMatrixException.hpp"
#include "../genlib/TransformationMatrix.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace renderer;
using namespace genlib;

ManagedObject* HeightMapFlipFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	DirectionTypeDefinition const* directionTypeDefinition = dynamic_cast<DirectionTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Direction")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(directionTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Direction direction = (Direction)NumberToInt(((NumberObject*)arguments[0])->GetValue());

	TransformationMatrix matrix;
	switch (direction)
	{
	case DIRECTION_HORIZONTAL:
		matrix.A11 = -1;
		matrix.A12 = 0;
		matrix.A21 = 0;
		matrix.A22 = 1;
		break;
	case DIRECTION_VERTICAL:
		matrix.A11 = 1;
		matrix.A12 = 0;
		matrix.A21 = 0;
		matrix.A22 = -1;
		break;
	default:
		break;
	}

	if (!matrix.IsInvertible())
	{
		throw InvalidTransformationMatrixException(location);
	}

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapTransformRenderingStep(location, argumentSlots, returnObjectSlot, matrix);
	vm->AddRenderingStep(location, renderingStep);

	return instance;
}