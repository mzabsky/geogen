#include "ArraySetFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* ArraySetFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	
	if (arguments.size() != 1){
		// a[b] = c form
		this->CheckArguments(location, 2, arguments);

		ArrayObject* thisArray = dynamic_cast<ArrayObject*>(instance);

		thisArray->Set(vm, location, arguments[1], arguments[0]);
		return arguments[0];
	}
	else {
		// a[] = b form
		ArrayObject* thisArray = dynamic_cast<ArrayObject*>(instance);

		thisArray->PushBack(vm, location, arguments[0]);
		return arguments[0];
	}
}