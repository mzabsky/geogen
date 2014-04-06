#pragma once

#include <string>
#include <map>

namespace geogen
{
	namespace runtime
	{
		class DynamicObject;
		class MemoryManager;

		class VariableTableItem
		{
			DynamicObject* value;
			bool isConst;
		public:
			VariableTableItem() : value(NULL), isConst(false)
			{
			};

			VariableTableItem(DynamicObject* value, bool isConst) : value(value), isConst(isConst)
			{				
			}

			inline bool IsConst() const { return this->isConst; }
			inline DynamicObject* GetValue() { return this->value; }
			inline DynamicObject const* GetValue() const { return this->value; }
			inline bool SetValue(DynamicObject* value)
			{
				if (this->isConst)
				{
					return false;
				}

				this->value = value;

				return true;
			}
		};

		class VariableTable
		{
		private:
			MemoryManager* memoryManager;
			std::map<std::string, VariableTableItem> table;

			// Non-copyable
			VariableTable(VariableTable const&) {}
			VariableTable& operator=(VariableTable const&) {};
		public:
			VariableTable(MemoryManager* memoryManager) : memoryManager(memoryManager) {}
			~VariableTable();

			typedef std::map<std::string, VariableTableItem const>::const_iterator const_iterator;
			typedef std::map<std::string, VariableTableItem>::iterator iterator;

			inline MemoryManager* GetMemoryManager() { return this->memoryManager; };
			inline MemoryManager const* GetMemoryManager() const { return this->memoryManager; };

			VariableTableItem const* GetVariable(std::string const& variableName) const;
			VariableTableItem* GetVariable(std::string const& variableName);
			bool SetVariable(std::string const& variableName, DynamicObject* value);
			bool IsVariableDeclared(std::string const& symbolName) const;
			bool DeclareVariable(std::string const& symbolName, DynamicObject* value, bool isConst);

			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }

			inline iterator Begin() { return this->table.begin(); }
			inline iterator End() { return this->table.end(); }			
		};
	}
}