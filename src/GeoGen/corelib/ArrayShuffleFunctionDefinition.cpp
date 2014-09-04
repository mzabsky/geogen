#include "ArrayShuffleFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../random/RandomSeed.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::random;

ManagedObject* ArrayShuffleFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 0);

	RandomSeed seed = arguments.size() > 0 ? (RandomSeed)NumberToInt(dynamic_cast<NumberObject*>(arguments[0])->GetValue()) : (RandomSeed)vm->GetCommonRandomSequence().NextUInt();

	RandomSeed finalSeed = CombineSeeds(vm->GetArguments().GetRandomSeed(), seed, CreateSeed(GG_STR("Array.Shuffle")));

	ArrayObject* thisArray = dynamic_cast<ArrayObject*>(instance);

	thisArray->Shuffle(vm, location, finalSeed);

	return vm->GetNull();
}