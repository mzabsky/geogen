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