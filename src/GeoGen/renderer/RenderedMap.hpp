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
#include "../Number.hpp"

namespace geogen
{
	namespace renderer
	{
		/// A single map produced by the Renderer.
		class RenderedMap
		{
		private:
			String mapName;
			Height* heightData;

			// Non-copyable
			RenderedMap(RenderedMap const&) {};
			RenderedMap& operator=(RenderedMap const&) {};
		public:

			/// Initializes a new instance of the RenderedMap class.		
			/// @param mapName Name of the map.
			/// @param heightData Pointer to the map data. This pointer will be owned by this object.
			RenderedMap(String const& mapName, Height* heightData) : mapName(mapName), heightData(heightData) {}

			/// Finalizes an instance of the RenderedMap class.
			~RenderedMap() { delete[] this->heightData; };

			/// Gets the map name.
			/// @return The map name.
			inline String GetMapName() const { return this->mapName; }

			/// Gets the actual height data.
			/// @return Pointer to the height data.
			inline Height* GetHeightData() const { return this->heightData; }
		};
	}
}