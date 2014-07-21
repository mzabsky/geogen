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