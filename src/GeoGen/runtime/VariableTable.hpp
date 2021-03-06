/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include <map>

#include "../Serializable.hpp"
#include "../String.hpp"
#include "VariableTableItem.hpp"

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;
		class MemoryManager;

		/// Table of variables declared in a single context.
		class VariableTable : public Serializable
		{
		private:
			MemoryManager* memoryManager;
			std::map<String, VariableTableItem> table;

			// Non-copyable
			VariableTable(VariableTable const&) {}
			VariableTable& operator=(VariableTable const&) {};
		public:

			/// Constructor.
			/// @param memoryManager The memory manager.
			VariableTable(MemoryManager* memoryManager) : memoryManager(memoryManager) {}

			/// Destructor. Removes a single reference from all contained objects.
			~VariableTable();

			/// Constant iterator.
			typedef std::map<String, VariableTableItem const>::const_iterator const_iterator;
			
			/// Iterator.
			typedef std::map<String, VariableTableItem>::iterator iterator;

			/// Gets memory manager.
			/// @return The memory manager.
			inline MemoryManager* GetMemoryManager() { return this->memoryManager; };
			
			/// Gets memory manager.
			/// @return The memory manager.
			inline MemoryManager const* GetMemoryManager() const { return this->memoryManager; };

			/// Gets a variable by its name.
			/// @param variableName Name of the variable.
			/// @return null if it fails, else the variable.
			VariableTableItem const* GetVariable(String const& variableName) const;
			
			/// Gets a variable by its name.
			/// @param variableName Name of the variable.
			/// @return null if it fails, else the variable.
			VariableTableItem* GetVariable(String const& variableName);

			/// Determines whether the table contains a variable with @a name.
			/// @param symbolName Name of the variable.
			/// @return true if variable is declared in this table, false otherwise.
			bool IsVariableDeclared(String const& symbolName) const;

			/// Declares a variable with specified value. Adds a reference to the value.
			/// @param symbolName Name of the variable.
			/// @param value The value. Do not use NULL, use VM NULL instead.
			/// @param isConst True if the variable is to be read only.
			/// @return true if it succeeds, false if it fails.
			bool DeclareVariable(String const& symbolName, ManagedObject* value, bool isConst);

			/// Gets the number of variables declared in the table.
			/// @return Size of the table.
			inline unsigned Size() const { return this->table.size(); }

			/// Gets an iterator pointing to the alphabetically first variable in the table.
			/// @return An iterator.
			inline const_iterator Begin() const { std::map<String, VariableTableItem>::const_iterator it = this->table.begin(); return (const_iterator&)(it); }

			/// Gets an ending iterator.
			/// @return An iterator.
			inline const_iterator End() const { std::map<String, VariableTableItem>::const_iterator it = this->table.end(); return (const_iterator&)(it); }

			/// Gets an iterator pointing to the alphabetically first variable in the table.
			/// @return An iterator.
			inline iterator Begin() { return this->table.begin(); }
			
			/// Gets an ending iterator.
			/// @return An iterator.
			inline iterator End() { return this->table.end(); }	

			virtual void Serialize(IOStream& stream) const;
		};
	}
}