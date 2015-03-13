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

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		/// List node in a metadata tree.
		class MetadataList : public MetadataValue
		{
		private:
			std::vector<MetadataValue*> table;
		public:
			/// A const iterator.
			typedef std::vector<MetadataValue const*>::const_iterator const_iterator;

			/// Constructor.
			/// @param location The code location.
			MetadataList(CodeLocation location) : MetadataValue(location) {}

			/// Adds an item to the list.
			/// @param  value The node to add. The list will assume ownership of this pointer.
			inline void AddItem(MetadataValue* value) {				
				this->table.push_back(value);
			};

			/// Gets the number of sub-nodes in the list.
			/// @return The size.
			inline unsigned Size() const { return this->table.size(); }

			/// Gets an iterator pointing to the first item in the list or the end iterator, if the list is empty.
			/// @return An iterator.
			inline const_iterator Begin() const { std::vector<MetadataValue*>::const_iterator it = this->table.begin(); return (const_iterator&)(it); }

			/// Gets the end iterator.
			/// @return An iterator.
			inline const_iterator End() const { std::vector<MetadataValue*>::const_iterator it = this->table.end(); return (const_iterator&)(it); }

			virtual MetadataType GetType() const { return METADATA_LIST; };

			/// Destructor. Destroys all sub-nodes.
			virtual ~MetadataList()
			{
				for(std::vector<MetadataValue*>::iterator it = this->table.begin(); it != this->table.end(); it++)
				{
					delete *it;
				}
			}

			virtual void Serialize(IOStream& stream) const
			{
				for (const_iterator it = this->Begin(); it != this->End(); it++)
				{
					(*it)->Serialize(stream); stream << std::endl;
				}
			}
		};
	}
}