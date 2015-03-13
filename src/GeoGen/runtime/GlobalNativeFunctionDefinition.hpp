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

#include <vector>

#include "FunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{	
	namespace runtime
	{
		class VirtualMachine;
		class ManagedObject;
		class TypeDefinition;

		/// Base class for global function definitions bound to a native function.
		class GlobalNativeFunctionDefinition : public FunctionDefinition
		{
		public:

			/// Constructor.
			/// @param name The function name.
			GlobalNativeFunctionDefinition(String const& name) : FunctionDefinition(name) {};

			virtual void Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const;

			virtual ManagedObject* CallNative(CodeLocation location, VirtualMachine* vm, std::vector<ManagedObject*> arguments) const = 0;

			virtual void Serialize(IOStream& stream) const;
		};
	}
}