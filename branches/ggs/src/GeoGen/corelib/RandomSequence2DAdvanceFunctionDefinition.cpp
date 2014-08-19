#include "RandomSequence2DAdvanceFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "RandomSequence2DTypeDefinition.hpp"
#include "RandomSequence2DObject.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::random;

ManagedObject* RandomSequence2DAdvanceFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	vector<TypeDefinition const*> expectedTypes;
	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	RandomSequence2D& sequence = dynamic_cast<RandomSequence2DObject*>(instance)->GetSequence();

	sequence.Advance();

	return vm->GetNull();
}
