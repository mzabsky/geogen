#pragma once

#include "ReferenceObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class VariableTableItem;

		class ScopeVariableReferenceObject : public ReferenceObject
		{
		private:
			VariableTableItem* variableTableItem;
		public:
			ScopeVariableReferenceObject(VirtualMachine* vm, TypeDefinition const* type, VariableTableItem* variableTableItem) : ReferenceObject(vm, type), variableTableItem(variableTableItem) {};
			//virtual ~ReferenceObject();

			virtual ManagedObject* GetReferencedObject(CodeLocation location, VirtualMachine* vm);
			virtual void SetReferencedObject(CodeLocation location, VirtualMachine* vm, ManagedObject* object);

			virtual String GetStringValue() const;
		};
	}
}