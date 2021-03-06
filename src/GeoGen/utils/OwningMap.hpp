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

#include "../String.hpp"

namespace geogen
{
	namespace utils
	{
		/// Base for classes that contains a collection of exclusively owned items indexed by string
		/// names.
		/// @tparam T Type of contained items (without *).
		template<typename T>		
		class OwningMap
		{
		protected:
			std::map<String, T*> table;
		public:

			/// Adds an item to the collection. The collection will assume ownership of the pointer.
			/// @param name The name.
			/// @param value The item to be added.
			/// @return true if the item was successfully added (the collection didn't contain an item with the same name yet).
			bool AddItem(String const& name, T* value)
			{
				if (this->table.find(name) != this->table.end()){
					return false;
				}

				this->table[name] = value;

				return true;
			}

			/// Gets the number of items in the collection.
			/// @return Number of items in the collection.
			inline unsigned Size() const { return this->table.size(); }

			/// Gets an iterator pointing to the first item in the collection (in order by names).
			/// @return An iterator.
			inline typename std::map<String, T const*>::const_iterator Begin() const { typename std::map<String, T*>::const_iterator it = this->table.begin(); return (typename std::map<String, T const*>::const_iterator&)(it); }

			/// Gets the ending iterator.
			/// @return An iterator.
			inline typename std::map<String, T const*>::const_iterator End() const { typename std::map<String, T*>::const_iterator it = this->table.end(); return (typename std::map<String, T const*>::const_iterator&)(it); }

			/// Gets an iterator pointing to the first item in the collection (in order by names).
			/// @return An iterator.
			inline typename std::map<String, T*>::iterator Begin() { return this->table.begin(); }
			
			/// Gets the ending iterator.
			/// @return An iterator.
			inline typename std::map<String, T*>::iterator End() { return this->table.end(); }

			/// Gets an iterator pointing to an item with given name, or an ending iterator if the collection doesn't contain item with than name.
			/// @param key The name.
			/// @return An iterator.
			inline typename std::map<String, T const*>::const_iterator FindItem(String const& key) const { typename std::map<String, T*>::const_iterator it = this->table.find(key); return (typename std::map<String, T const*>::const_iterator&)(it); }
			
			/// Gets an iterator pointing to an item with given name, or an ending iterator if the collection doesn't contain item with than name.
			/// @param key The name.
			/// @return An iterator.
			inline typename std::map<String, T*>::iterator FindItem(String const& key) { return this->table.find(key); }

			/// Determines whether the collection contains item with given name.
			/// @param key The name.
			/// @return true if the collection contains the name, false otherwise.
			inline bool ContainsItem(String const& key) const { return this->FindItem(key) != this->End(); };

			/// Gets an item with given name.
			/// @param key The name.
			/// @return The item, or NULL if not found.
			T const* GetItem(String const& key) const
			{
				typename std::map<String, T const*>::const_iterator it = this->FindItem(key);
				if (it == this->End())
				{
					return NULL;
				}

				return it->second;
			};

			/// Gets an item with given name.
			/// @param key The name.
			/// @return The item, or NULL if not found.
			T* GetItem(String const& key)
			{
				typename std::map<String, T*>::iterator it = this->FindItem(key);
				if (it == this->End())
				{
					return NULL;
				}

				return it->second;
			};

			/// Virtual destructor. Destroys all owned items.
			virtual ~OwningMap()
			{
				for (typename std::map<String, T*>::iterator it = this->table.begin(); it != this->table.end(); it++)
				{
					delete it->second;
				}
			}
		};
	}
}