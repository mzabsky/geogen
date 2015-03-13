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

#include "../String.hpp"
#include "../Serializable.hpp"

namespace geogen 
{
	namespace runtime
	{
		class ManagedObject;
		class VirtualMachine;

		/// Base class for definitions of pre-defined variables. These are often not strictly necessary though - any code can insert items straight into an appropriate VariableTable.
		class VariableDefinition : public Serializable
		{
		private:
			String name;

			bool isConstant;
		protected:
		public:			

			/// Constructor.
			/// @param name The variable name.
			/// @param isConstant True if the variable is to be read-only.
			VariableDefinition(String const& name, bool isConstant) :
				name(name), isConstant(isConstant)
			{}

			/// Gets the name.
			/// @return The name.
			inline String const& GetName() const { return this->name; };

			/// Query if the variable is to be read-only.
			/// @return true if the variable is read-only.
			inline bool IsConstant() const { return this->isConstant; };

			/// Creates default value of the variable (called by the default implementation Initialize).
			/// @param vm The virtual machine.
			/// @return The default value.
			virtual ManagedObject* CreateDefaultValue(VirtualMachine* vm) const { return NULL; }

			/// Initializes this variable. Executed when the VirtualMachine is starting up.
			/// @param vm The virtual machine.
			virtual void Initialize(VirtualMachine* vm) const;

			virtual void Serialize(IOStream& stream) const;
		};
	}
}