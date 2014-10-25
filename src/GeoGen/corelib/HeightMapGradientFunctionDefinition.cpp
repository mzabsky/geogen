#include "HeightMapGradientFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "PointTypeDefinition.hpp"
#include "PointObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapGradientRenderingStep.hpp"
#include "HeightOverflowException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapGradientFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	PointTypeDefinition const* pointTypeDefinition = dynamic_cast<PointTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Point")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(pointTypeDefinition);
	expectedTypes.push_back(pointTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Point source = dynamic_cast<PointObject*>(arguments[0])->GetAbsolutePoint(vm, location);
	Point destination = dynamic_cast<PointObject*>(arguments[1])->GetAbsolutePoint(vm, location);

	Number numberFromHeight = ((NumberObject*)arguments[2])->GetValue();
	Height fromHeight;
	if (!TryNumberToHeight(numberFromHeight, fromHeight))
	{
		throw HeightOverflowException(location);
	}

	Number numberToHeight = ((NumberObject*)arguments[3])->GetValue();
	Height toHeight;
	if (!TryNumberToHeight(numberToHeight, toHeight))
	{
		throw HeightOverflowException(location);
	}

	ManagedObject* returnObject = dynamic_cast<HeightMapTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightMapGradientRenderingStep(location, argumentSlots, returnObjectSlot, source, destination, fromHeight, toHeight);
	vm->AddRenderingStep(location, renderingStep);

	return returnObject;
}