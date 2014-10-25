#include "ArrayGetRefFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "ReferenceTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;

ManagedObject* ArrayGetRefFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	ReferenceTypeDefinition const* referenceTypeDefinition = dynamic_cast<ReferenceTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("<Reference>")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	this->CheckArguments(location, 1, arguments);

	ArrayObject* thisArray = dynamic_cast<ArrayObject*>(instance);

	// Do not store the key from the stack - it may be a temporary object that is just equivalent to the actual key
	ManagedObject* actualKey = thisArray->GetActualKey(vm, location, arguments[0]);	

	return referenceTypeDefinition->CreateArrayItemReferenceInstance(vm, thisArray, actualKey);
}