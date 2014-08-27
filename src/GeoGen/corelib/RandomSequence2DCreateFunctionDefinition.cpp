#include "RandomSequence2DCreateFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "RandomSequence2DTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "RandomSequence2DObject.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;
using namespace geogen::random;

ManagedObject* RandomSequence2DCreateFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	RandomSequence2DTypeDefinition const* sequenceTypeDefinition = dynamic_cast<RandomSequence2DTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("RandomSequence2D")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 0);

	RandomSeed passedSeed = 0;
	if (arguments.size() > 0)
	{
		passedSeed = (RandomSeed)NumberToInt(dynamic_cast<NumberObject*>(arguments[0])->GetValue());
	}
	else
	{
		passedSeed = (RandomSeed)vm->GetCommonRandomSequence().NextUInt();
	}
	
	RandomSeed actualSeed = CombineSeeds(passedSeed, vm->GetArguments().GetRandomSeed(), CreateSeed(GG_STR("RandomSequence2D")));

	return sequenceTypeDefinition->CreateInstance(vm, actualSeed);
}