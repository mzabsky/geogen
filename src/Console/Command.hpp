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

#include <GeoGen/GeoGen.hpp>

namespace geogen
{
	namespace console
	{
		class Command
		{
		protected:
			std::vector<String> cues;
		public:
			virtual String GetName() const = 0;
			inline std::vector<String> const& GetCues() const { return this->cues; };
			virtual String GetHelpString() const = 0;

			virtual ~Command() {}
		};
	}
}