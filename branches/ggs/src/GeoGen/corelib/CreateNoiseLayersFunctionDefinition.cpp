#include "CreateNoiseLayersFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/IncorrectTypeException.hpp"
#include "../genlib/NoiseLayersFactory.hpp"
#include "NumberTypeDefinition.hpp"
#include "ArrayTypeDefinition.hpp"
#include "MaxLessThanMinException.hpp"
#include "InvalidPersistenceException.hpp"
#include "ArrayObject.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::genlib;

ManagedObject* CreateNoiseLayersFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	ArrayTypeDefinition const* arrayTypeDefinition = dynamic_cast<ArrayTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Array")));

	vector<TypeDefinition const*> expectedParameters;
	expectedParameters.push_back(numberTypeDefinition);
	expectedParameters.push_back(numberTypeDefinition);
	expectedParameters.push_back(numberTypeDefinition);
	//expectedParameters.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedParameters, arguments, 0);

	Size1D minimumFeatureSize;
	Size1D maximumFeatureSize;
	double persistence;
	if (arguments.size() == 0)
	{
		minimumFeatureSize = 1;
		maximumFeatureSize = 256;
		persistence = 0.5;
	}
	else if (arguments.size() == 1)
	{
		minimumFeatureSize = 1;
		maximumFeatureSize = (Size1D)NumberToInt(dynamic_cast<NumberObject*>(arguments[0])->GetValue());
		persistence = 0.5;
	}
	else 
	{
		minimumFeatureSize = (Size1D)NumberToInt(dynamic_cast<NumberObject*>(arguments[0])->GetValue());
		maximumFeatureSize = (Size1D)NumberToInt(dynamic_cast<NumberObject*>(arguments[1])->GetValue());
		persistence = arguments.size() > 2 ? dynamic_cast<NumberObject*>(arguments[2])->GetValue() : 0.50;
	}
	
	if (minimumFeatureSize > maximumFeatureSize)
	{
		throw MaxLessThanMinException(location);
	}

	if (persistence < 0.10 || persistence > 0.90)
	{
		throw InvalidPersistenceException(location);
	}

	map<Size1D, Number> layers;
	Size1D currentWaveLength = maximumFeatureSize;
	Number currentAmplitude = 1;
	while (currentWaveLength > minimumFeatureSize)
	{
		layers[currentWaveLength] = currentAmplitude;

		currentWaveLength /= 2;
		currentAmplitude *= 1 - persistence;
	}

	Number sum = 0;
	for (map<Size1D, Number>::iterator it = layers.begin(); it != layers.end(); it++)
	{
		sum += it->second;
	}

	ArrayObject* array = dynamic_cast<ArrayObject*>(arrayTypeDefinition->CreateInstance(vm));
	for (map<Size1D, Number>::iterator it = layers.begin(); it != layers.end(); it++)
	{
		array->Set(vm, location, numberTypeDefinition->CreateInstance(vm, it->first), numberTypeDefinition->CreateInstance(vm, it->second / sum));
	}

	return array;
}