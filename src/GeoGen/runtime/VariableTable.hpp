#pragma once

#include <map>

#include "../String.hpp"
#include "VariableTableItem.hpp"

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;
		class MemoryManager;

		class VariableTable
		{
		private:
			MemoryManager* memoryManager;
			std::map<String, VariableTableItem> table;

			// Non-copyable
			VariableTable(VariableTable const&) {}
			VariableTable& operator=(VariableTable const&) {};
		public:
			VariableTable(MemoryManager* memoryManager) : memoryManager(memoryManager) {}
			~VariableTable();

			typedef std::map<String, VariableTableItem const>::const_iterator const_iterator;
			typedef std::map<String, VariableTableItem>::iterator iterator;

			inline MemoryManager* GetMemoryManager() { return this->memoryManager; };
			inline MemoryManager const* GetMemoryManager() const { return this->memoryManager; };

			VariableTableItem const* GetVariable(String const& variableName) const;
			VariableTableItem* GetVariable(String const& variableName);
			bool IsVariableDeclared(String const& symbolName) const;
			bool DeclareVariable(String const& symbolName, ManagedObject* value, bool isConst);

			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }

			inline iterator Begin() { return this->table.begin(); }
			inline iterator End() { return this->table.end(); }			
		};
	}
}