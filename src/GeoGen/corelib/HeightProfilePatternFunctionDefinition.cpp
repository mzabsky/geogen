#include "HeightProfilePatternFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "NumberTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "CoordinateObject.hpp"
#include "HeightProfilePatternRenderingStep.hpp"
#include "UnknownRelativeCoordinateDirectionException.hpp"
#include "SizeOverflowException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfilePatternFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(this->GetOwningTypeDefinition());
	expectedTypes.push_back(coordinateTypeDefinition);
	expectedTypes.push_back(coordinateTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	CoordinateObject* startObject = dynamic_cast<CoordinateObject*>(arguments[1]);
	CoordinateObject* lengthObject = dynamic_cast<CoordinateObject*>(arguments[2]);

	if (startObject->IsRelative() || lengthObject->IsRelative())
	{
		throw UnknownRelativeCoordinateDirectionException(location);
	}

	Coordinate start = NumberToInt(startObject->GetValue());

	Number numberLength = lengthObject->GetValue();
	Size1D length;
	if (!TryNumberToSize(numberLength, length))
	{
		throw SizeOverflowException(location);
	}

	ManagedObject* returnObject = dynamic_cast<HeightProfileTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightProfilePatternRenderingStep(location, argumentSlots, returnObjectSlot, Interval(start, length));
	vm->GetRenderingSequence().AddStep(renderingStep);

	return returnObject;
}