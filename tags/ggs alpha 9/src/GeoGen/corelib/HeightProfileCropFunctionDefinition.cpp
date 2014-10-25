#include "HeightProfileCropFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightProfileCropRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "CoordinateObject.hpp"
#include "HeightOverflowException.hpp"
#include "SizeOverflowException.hpp"
#include "UnknownRelativeCoordinateDirectionException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfileCropFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(coordinateTypeDefinition);
	expectedTypes.push_back(coordinateTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 2);

	CoordinateObject* startObject = dynamic_cast<CoordinateObject*>(arguments[0]);
	CoordinateObject* endObject = dynamic_cast<CoordinateObject*>(arguments[1]);

	if (startObject->IsRelative() || endObject->IsRelative())
	{
		throw UnknownRelativeCoordinateDirectionException(location);
	}

	Coordinate start = (Coordinate)startObject->GetValue();
	Coordinate end = (Coordinate)endObject->GetValue();

	Number numberHeight = arguments.size() > 2 ? ((NumberObject*)arguments[2])->GetValue() : 0;
	Height height;
	if (!TryNumberToHeight(numberHeight, height))
	{
		throw HeightOverflowException(location);
	}

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightProfileCropRenderingStep(location, argumentSlots, returnObjectSlot, Interval::FromCoordinates(start, end), height);
	vm->AddRenderingStep(location, renderingStep);

	return instance;
}