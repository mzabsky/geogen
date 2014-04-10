#include "AssignmentOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/DynamicObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

DynamicObject* AssignmentOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<DynamicObject*> arguments) const
{
	this->CheckArguments(location, 1, arguments);

	return arguments[0]->GetType()->Copy(arguments[0]);
}