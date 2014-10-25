#include "RandomSequence2DGetNumberFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "RandomSequence2DTypeDefinition.hpp"
#include "PointTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "RandomSequence2DObject.hpp"
#include "PointObject.hpp"
#include "MaxLessThanMinException.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace random;

ManagedObject* RandomSequence2DGetNumberFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	PointTypeDefinition const* pointTypeDefinition = dynamic_cast<PointTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Point")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(pointTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 1);

	Point point = ((PointObject*)arguments[0])->GetAbsolutePoint(vm, location);

	RandomSequence2D& sequence = dynamic_cast<RandomSequence2DObject*>(instance)->GetSequence();

	Number n;
	if (arguments.size() > 1)
	{
		Number min = dynamic_cast<NumberObject*>(arguments[1])->GetValue();
		Number max = dynamic_cast<NumberObject*>(arguments[2])->GetValue();

		if (min > max)
		{
			throw MaxLessThanMinException(location);
		}

		n = sequence.GetDouble(point) * (max - min) + min;
	}
	else
	{
		n = sequence.GetDouble(point);
	}

	return numberTypeDefinition->CreateInstance(vm, n);
}