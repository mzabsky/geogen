#pragma once

#include "../genlib/HeightMap.hpp"
#include "../utils/OwningMap.hpp"
#include "../Number.hpp"

namespace geogen
{
	namespace renderer
	{
		/// <summary> A table used to temporarily store rendered maps. </summary>
		class RenderedMapTable : public utils::OwningMap<genlib::HeightMap>
		{
		public:
			typedef std::map<String, genlib::HeightMap*>::iterator iterator;
			typedef std::map<String, genlib::HeightMap const*>::const_iterator const_iterator;

			/// Removes an entry with given name from the table.
			/// @param name The name.
			bool RemoveItem(String name);

			/// Removes all items from the table.
			void Clear();
		};
	}
}