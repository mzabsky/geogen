#pragma once

#include "ReferenceObject.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
		class ManagedObject;
		class VariableTableItem;
		class TypeDefinition;
	}

	namespace corelib
	{
		class ScopeVariableReferenceObject : public ReferenceObject
		{
		private:
			runtime::VariableTableItem* variableTableItem;
		public:
			ScopeVariableReferenceObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type, runtime::VariableTableItem* variableTableItem) : ReferenceObject(vm, type), variableTableItem(variableTableItem) {};
			//virtual ~ReferenceObject();

			virtual ManagedObject* GetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm);
			virtual void SetReferencedObject(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* object);

			virtual String GetStringValue() const;
		};
	}
}