#include "HeightMapPatternFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "PointTypeDefinition.hpp"
#include "PointObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapPatternRenderingStep.hpp"
#include "SizeOverflowException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapPatternFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	PointTypeDefinition const* pointTypeDefinition = dynamic_cast<PointTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Point")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(this->GetOwningTypeDefinition());
	expectedTypes.push_back(pointTypeDefinition);
	expectedTypes.push_back(pointTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Point origin = dynamic_cast<PointObject*>(arguments[1])->GetAbsolutePoint(vm, location);
	Point toPoint = dynamic_cast<PointObject*>(arguments[2])->GetAbsolutePoint(vm, location);

	ManagedObject* returnObject = dynamic_cast<HeightMapTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightMapPatternRenderingStep(location, argumentSlots, returnObjectSlot, Rectangle(origin, toPoint));
	vm->GetRenderingSequence().AddStep(renderingStep);

	return returnObject;
}