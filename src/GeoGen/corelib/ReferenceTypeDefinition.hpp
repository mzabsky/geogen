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