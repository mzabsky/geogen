#pragma once

#include "../Number.hpp"
#include "TypeDefinition.hpp"
#include "VirtualMachine.hpp"

namespace geogen
{
	namespace corelib
	{
		class ArrayObject;
	}

	namespace runtime
	{
		class ReferenceTypeDefinition : public TypeDefinition
		{
		public:		
			ReferenceTypeDefinition() : TypeDefinition(GG_STR("<Reference>")) {}
			ManagedObject* CreateScopeReferenceInstance(VirtualMachine* vm, VariableTableItem* variableTableItem) const;
			ManagedObject* CreateArrayItemReferenceInstance(VirtualMachine* vm, corelib::ArrayObject* array, ManagedObject* key) const;

			virtual ManagedObject* Copy(VirtualMachine* vm, ManagedObject* a) const;
		};
	}
}