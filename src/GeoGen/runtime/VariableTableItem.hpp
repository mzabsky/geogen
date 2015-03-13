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

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;
		class MemoryManager;
		class VirtualMachine;

		/// Item in VariableTable, representing a single defined variable.
		class VariableTableItem
		{
			ManagedObject* value;
			bool isConst;
		public:
			/// Constructs a declared, but undefined variable.
			VariableTableItem() : value(NULL), isConst(false)
			{
			};

			/// Constructs a variable with a defined value.
			/// @param value The value.
			/// @param isConst True if the variable is to be read-only, false if it is to be writable.
			VariableTableItem(ManagedObject* value, bool isConst) : value(value), isConst(isConst)
			{
			}

			/// Determines whether the variable is read-only.
			/// @return true if constant, false if not.
			inline bool IsConst() const { return this->isConst; }

			/// Gets the value.
			/// @return The value.
			inline ManagedObject* GetValue() { return this->value; }
			
			/// Gets the value.
			/// @return The value.
			inline ManagedObject const* GetValue() const { return this->value; }

			/// Sets the value.
			/// @param vm If non-null, the virtual memory.
			/// @param value The value.
			/// @return true if it succeeds (if the variable is writable), false if it fails (if the variable is read-only).
			bool SetValue(VirtualMachine* vm, ManagedObject* value);
		};
	}
}