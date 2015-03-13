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

#include "../Point.hpp"
#include "../Number.hpp"
#include "../runtime/TypeDefinition.hpp"
#include "../random/RandomSeed.hpp"

namespace geogen
{
	namespace corelib
	{
		class RandomSequence2DTypeDefinition : public runtime::TypeDefinition
		{
		public:
			RandomSequence2DTypeDefinition();

			virtual runtime::ManagedObject* CreateInstance(runtime::VirtualMachine* vm, random::RandomSeed randomSeed) const;

			virtual runtime::ManagedObject* Copy(runtime::VirtualMachine* vm, runtime::ManagedObject* a) const;
		};
	}
}