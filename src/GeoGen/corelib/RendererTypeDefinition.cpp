#include "RendererTypeDefinition.hpp"
#include "../runtime/RendererObjectStub.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

ManagedObject* RendererTypeDefinition::CreateInstance(VirtualMachine* vm) const
{
	auto_ptr<ManagedObject> object(new RendererObjectStub(vm, this));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

runtime::ManagedObject* RendererTypeDefinition::Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const
{
	return a;
}