#include "HeightMapDrawLineFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "PointTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "PointObject.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapDrawLineRenderingStep.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapDrawLineFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	PointTypeDefinition const* pointTypeDefinition = dynamic_cast<PointTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Point")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(pointTypeDefinition);
	expectedTypes.push_back(pointTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Point start = dynamic_cast<PointObject*>(arguments[0])->GetAbsolutePoint(vm, location);
	Point end = dynamic_cast<PointObject*>(arguments[1])->GetAbsolutePoint(vm, location);

	Number heightNumber = dynamic_cast<NumberObject*>(arguments[2])->GetValue();
	Height height;
	if (!TryNumberToHeight(heightNumber, height))
	{
		throw HeightOverflowException(location);
	}

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapDrawLineRenderingStep(location, argumentSlots, returnObjectSlot, start, end, height);
	vm->AddRenderingStep(location, renderingStep);

	return instance;
}