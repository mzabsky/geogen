#include "ArrayItemReferenceObject.hpp"
#include "../corelib/ArrayObject.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

ManagedObject* ArrayItemReferenceObject::GetReferencedObject(CodeLocation location, VirtualMachine* vm)
{
	return this->array->Get(vm, location, this->key);
}

void ArrayItemReferenceObject::SetReferencedObject(CodeLocation location, VirtualMachine* vm, ManagedObject* object)
{
	this->array->Set(vm, location, this->key, object);
}

String ArrayItemReferenceObject::GetStringValue() const
{
	StringStream ss;
	ss << GG_STR("&[array #") << this->array->GetObjectId() << ", key #" << this->key->GetObjectId() << GG_STR("]");
	return ss.str();
}