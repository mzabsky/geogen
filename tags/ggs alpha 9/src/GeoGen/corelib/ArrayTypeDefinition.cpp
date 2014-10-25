#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayCountFunctionDefinition.hpp"
#include "ArrayEmptyFunctionDefinition.hpp"
#include "ArrayPushBackFunctionDefinition.hpp"
#include "ArrayFrontBackFunctionDefinition.hpp"
#include "ArrayGetFunctionDefinition.hpp"
#include "ArrayGetRefFunctionDefinition.hpp"
#include "ArraySetFunctionDefinition.hpp"
#include "ArrayContainsFunctionDefinition.hpp"
#include "ArrayFromListFunctionDefinition.hpp"
#include "ArrayRemoveFunctionDefinition.hpp"
#include "ArrayRepeatFunctionDefinition.hpp"
#include "ArrayGetKeyByIndexFunctionDefinition.hpp"
#include "ArrayGetValueByIndexFunctionDefinition.hpp"
#include "ArraySortByKeysFunctionDefinition.hpp"
#include "ArraySortByValuesFunctionDefinition.hpp"
#include "ArrayShuffleFunctionDefinition.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

const String ArrayTypeDefinition::UNSET_KEY_VARIABLE_NAME = GG_STR("<unsetKey>");

ArrayTypeDefinition::ArrayTypeDefinition() : TypeDefinition(GG_STR("Array"))
{	
	this->GetStaticFunctionDefinitions().AddItem(new ArrayEmptyFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new ArrayFromListFunctionDefinition(this));

	this->GetFunctionDefinitions().AddItem(new ArrayCountFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArrayPushBackFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArrayGetFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArrayGetRefFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArraySetFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(ArrayFrontBackFunctionDefinition::Create(ArrayFrontBackFunctionDefinition::FRONT, this));
	this->GetFunctionDefinitions().AddItem(ArrayFrontBackFunctionDefinition::Create(ArrayFrontBackFunctionDefinition::BACK, this));
	this->GetFunctionDefinitions().AddItem(ArrayContainsFunctionDefinition::Create(ArrayContainsFunctionDefinition::CONTAINS_KEY, this));
	this->GetFunctionDefinitions().AddItem(ArrayContainsFunctionDefinition::Create(ArrayContainsFunctionDefinition::CONTAINS_VALUE, this));
	this->GetFunctionDefinitions().AddItem(ArrayRemoveFunctionDefinition::Create(ArrayRemoveFunctionDefinition::REMOVE_KEY, this));
	this->GetFunctionDefinitions().AddItem(ArrayRemoveFunctionDefinition::Create(ArrayRemoveFunctionDefinition::REMOVE_VALUE, this));
	this->GetFunctionDefinitions().AddItem(new ArrayRepeatFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArrayGetKeyByIndexFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArrayGetValueByIndexFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArraySortByKeysFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArraySortByValuesFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new ArrayShuffleFunctionDefinition(this));
}


void ArrayTypeDefinition::Initialize(VirtualMachine* vm) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	if (!vm->GetGlobalVariableTable().DeclareVariable(GG_STR(UNSET_KEY_VARIABLE_NAME), numberTypeDefinition->CreateInstance(vm, 0), true))
	{
		throw InternalErrorException(GG_STR("Could not declare unset key marker."));
	}

	TypeDefinition::Initialize(vm);
}

ManagedObject* ArrayTypeDefinition::CreateInstance(VirtualMachine* vm) const
{
	ArrayObject* object = new ArrayObject(vm, this);
	vm->GetMemoryManager().RegisterObject(object);
	return object;
}

//bool ArrayTypeDefinition::InstanceEqualsTo(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const
//{
//	if (a->GetType() != this)
//	{
//		throw InternalErrorException(GG_STR("Using InstanceEqualsTo on object of incorrect type."));
//	}
//
//	if (b->GetType() == this)
//	{
//		ArrayObject const* arrayA = dynamic_cast<ArrayObject const*>(a);
//		ArrayObject const* arrayB = dynamic_cast<ArrayObject const*>(b);
//		
//		if (arrayA->Count() != arrayB->Count())
//		{
//			return false;
//		}
//
//		ArrayObject::const_iterator itB = arrayB->Begin();
//		for (ArrayObject::const_iterator itA = arrayA->Begin(); itA != arrayA->End(); itA++)
//		{
//			/*if (!itA->first->GetType()->InstanceEqualsTo(itA->first, itB->first))
//			{
//				return false;
//			}
//
//			if (!itA->second->GetType()->InstanceEqualsTo(itA->second, itB->second))
//			{
//				return false;
//			}*/
//
//			itB++;
//		}
//	}
//
//	return TypeDefinition::InstanceLessThan(a, b);
//}

/*bool ArrayTypeDefinition::InstanceLessThan(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceLessThan on object of incorrect type."));
	}

	return TypeDefinition::InstanceLessThan(a, b);
}*/

ManagedObject* ArrayTypeDefinition::Copy(VirtualMachine* vm, runtime::ManagedObject* a) const
{
	return a;
}