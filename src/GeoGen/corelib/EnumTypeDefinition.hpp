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

#include "../String.hpp"
#include "../Number.hpp"
#include "../runtime/TypeDefinition.hpp"

namespace geogen
{
	namespace runtime 
	{
		class ManagedObject;
		class VirtualMachine;
	}

	namespace corelib
	{
		class EnumTypeDefinition : public runtime::TypeDefinition
		{
		public:
			typedef std::map<String, int> const ValueDefinitions;
		private:
			ValueDefinitions valueDefinitions;
		public:
			EnumTypeDefinition(String const& name, ValueDefinitions values);

			virtual void Initialize(runtime::VirtualMachine* vm) const;

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm, Number value) const;

			virtual bool IsEnumType() const { return true; };

			virtual bool InstanceLessThan(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(runtime::ManagedObject const* a, runtime::ManagedObject const* b) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;

			inline ValueDefinitions const& GetValueDefinitions() const { return this->valueDefinitions; }

			runtime::ManagedObject* GetValueByInt(runtime::VirtualMachine* vm, int intValue) const;
			bool IsValueIntDefined(int intValue) const;
			int GetDefaultValueInt() const;
			bool IsValueStringDefined(String stringValue) const;
		};
	}
}