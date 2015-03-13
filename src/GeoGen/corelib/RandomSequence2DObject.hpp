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

#include "../runtime/ManagedObject.hpp"
#include "../random/RandomSequence2D.hpp"

namespace geogen
{
	namespace runtime
	{
		class RandomSequence2DObject : public ManagedObject
		{
		private:
			random::RandomSeed seed;
			random::RandomSequence2D sequence;
		public:
			RandomSequence2DObject(VirtualMachine* vm, TypeDefinition const* type, random::RandomSeed randomSeed) : ManagedObject(vm, type), sequence(randomSeed), seed(randomSeed)
			{
			};
			inline random::RandomSequence2D& GetSequence() { return this->sequence; }

			virtual String GetStringValue() const
			{
				StringStream ss;
				ss << GG_STR("Seed[") << this->seed << GG_STR("]");

				return ss.str();
			}
		};
	}
}