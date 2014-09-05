#include "ReferenceObject.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

ReferenceObject::~ReferenceObject()
{
	//this->GetReferencedObject()->RemoveRef(this->GetMemberVariableTable().GetMemoryManager());
}