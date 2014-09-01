#pragma once

#include "../genlib/HeightMap.hpp"
#include "../utils/OwningMap.hpp"
#include "../Number.hpp"

namespace geogen
{
	namespace renderer
	{
		/// A table used to temporarily store rendered maps.
		class RenderedMapTable : public utils::OwningMap<genlib::HeightMap>, public Serializable
		{
		public:
			typedef std::map<String, genlib::HeightMap*>::iterator iterator;
			typedef std::map<String, genlib::HeightMap const*>::const_iterator const_iterator;

			/// Removes an entry with given name from the table.
			/// @param name The name.
			bool RemoveItem(String name);

			/// Removes all items from the table.
			void Clear();

			virtual void Serialize(IOStream& stream) const;
		};
	}
}