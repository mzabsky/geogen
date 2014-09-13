#include "RandomSequence2DTypeDefinition.hpp"
#include "../runtime/RendererObjectStub.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "RandomSequence2DObject.hpp"
#include "RandomSequence2DCreateFunctionDefinition.hpp"
#include "RandomSequence2DAdvanceFunctionDefinition.hpp"
#include "RandomSequence2DGetNumberFunctionDefinition.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace random;
using namespace std;

RandomSequence2DTypeDefinition::RandomSequence2DTypeDefinition() : TypeDefinition(GG_STR("RandomSequence2D"))
{
	this->GetFunctionDefinitions().AddItem(new RandomSequence2DAdvanceFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new RandomSequence2DGetNumberFunctionDefinition(this));

	this->GetStaticFunctionDefinitions().AddItem(new RandomSequence2DCreateFunctionDefinition(this));
}

ManagedObject* RandomSequence2DTypeDefinition::CreateInstance(VirtualMachine* vm, RandomSeed randomSeed) const
{
	auto_ptr<ManagedObject> object(new RandomSequence2DObject(vm, this, randomSeed));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

runtime::ManagedObject* RandomSequence2DTypeDefinition::Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const
{
	return a;
}