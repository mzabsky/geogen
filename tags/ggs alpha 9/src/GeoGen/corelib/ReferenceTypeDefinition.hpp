#pragma once

#include "../Number.hpp"
#include "../runtime/TypeDefinition.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
		class VariableTableItem;
	}

	namespace corelib
	{
		class ArrayObject;

		class ReferenceTypeDefinition : public runtime::TypeDefinition
		{
		public:		
			ReferenceTypeDefinition() : TypeDefinition(GG_STR("<Reference>")) {}
			runtime::ManagedObject* CreateScopeReferenceInstance(runtime::VirtualMachine* vm, runtime::VariableTableItem* variableTableItem) const;
			runtime::ManagedObject* CreateArrayItemReferenceInstance(runtime::VirtualMachine* vm, corelib::ArrayObject* array, runtime::ManagedObject* key) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}