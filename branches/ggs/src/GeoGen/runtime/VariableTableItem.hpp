#pragma once

#include <map>

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;
		class MemoryManager;
		class VirtualMachine;

		class VariableTableItem
		{
			ManagedObject* value;
			bool isConst;
		public:
			VariableTableItem() : value(NULL), isConst(false)
			{
			};

			VariableTableItem(ManagedObject* value, bool isConst) : value(value), isConst(isConst)
			{
			}

			inline bool IsConst() const { return this->isConst; }
			inline ManagedObject* GetValue() { return this->value; }
			inline ManagedObject const* GetValue() const { return this->value; }
			bool SetValue(VirtualMachine* vm, ManagedObject* value);
		};
	}
}