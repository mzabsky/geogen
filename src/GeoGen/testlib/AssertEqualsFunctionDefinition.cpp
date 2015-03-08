#include "AssertEqualsFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "AssertionFailedException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::testlib;
using namespace geogen::runtime;

ManagedObject* AssertEqualsFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	this->CheckArguments(location, 2, arguments);

	if (!arguments[0]->GetType()->InstanceEqualsTo(arguments[0], arguments[1]))
	{
		throw AssertionFailedException(location, arguments[0]->ToString(), arguments[1]->ToString());
	}

	return vm->GetNull();
}