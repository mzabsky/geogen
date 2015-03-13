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

#include "../runtime/TypeDefinition.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
		class TypeDefinition;
	}

	namespace corelib
	{
		/// <summary> Base type for objects that represent <see cref="RendererObject"/>s during execution of the <see cref="VirtualMachine"/>. </summary>
		class RendererTypeDefinition : public runtime::TypeDefinition
		{
		public:

			/// <summary> Initializes a new instance of the RendererTypeDefinition class. </summary>
			/// <param name="name"> The type name. </param>
			RendererTypeDefinition(String const& name) : TypeDefinition(name) {}

			/// <summary> Creates a new instance of this type. </summary>
			/// <param name="vm"> The virtual machine. </param>
			/// <returns> New instance of this type. This instance will be already registered with the <see cref="MemoryManager"/>.</returns>
			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}