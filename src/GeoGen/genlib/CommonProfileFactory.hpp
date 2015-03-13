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

#include "HeightProfile.hpp"

namespace geogen
{
	namespace genlib
	{
		typedef std::map<Size1D, Height> NoiseLayers;

		/// Factory class creating well known HeightProfile instances.
		class CommonProfileFactory
		{
		private:
			// This is a static class, instance creation is forbidden.
			CommonProfileFactory(){};
		public:
			static HeightProfile CreateGlaciationProfile(Size1D size, double strength);
			static HeightProfile CreateStratificationProfile(Size1D size, unsigned numberOfStrata, double steepness, double smoothness);

			static HeightProfile CreateMirroredProfile(HeightProfile& profile);
		};
	}
}
