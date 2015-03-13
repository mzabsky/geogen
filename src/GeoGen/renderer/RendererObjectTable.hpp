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
#include "../Serializable.hpp"

namespace geogen
{
	namespace renderer
	{
		class RendererObject;

		/// A table of RendererObject items stored in a fixed number of slots. Owns the contained
		/// pointers.
		class RendererObjectTable : public Serializable
		{
		private:
			std::vector<RendererObject*> table;

			// Non-copyable
			RendererObjectTable(RendererObjectTable const&) {};
			RendererObjectTable& operator=(RendererObjectTable const&) {};
		public:
			typedef std::vector<RendererObject const*>::const_iterator const_iterator;
			typedef std::vector<RendererObject*>::iterator iterator;

			/// Initializes this instance.
			/// @param size The number of slots in the table.
			RendererObjectTable(unsigned size) : table(std::vector<RendererObject*>(size, NULL)) {}

			/// Finalizes an instance of the RendererObject class. Releases the wrapped pointers.
			~RendererObjectTable();

			/// Gets the number oslots in the table.
			/// @return The size.
			inline unsigned GetSize() { return this->table.size(); }

			inline const_iterator Begin() const { std::vector<RendererObject*>::const_iterator it = this->table.begin(); return (const_iterator&)(it); }
			inline const_iterator End() const { std::vector<RendererObject*>::const_iterator it = this->table.end(); return (const_iterator&)(it); }
			inline iterator Begin() { return this->table.begin(); }
			inline iterator End() { return this->table.end(); }

			/// Gets an object from the specified slot (alias for GetObjectBySlot).
			/// @param slot The slot.
			/// @return The object stored in the specified slot.
			inline RendererObject* GetObject(unsigned slot) { return this->GetObjectBySlot(slot); };

			/// Gets an object from the specified slot.
			/// @param slot The slot.
			/// @return The object stored in the specified slot.
			RendererObject* GetObjectBySlot(unsigned slot);

			/// Stores the object into the specified slot.
			/// @param slot			   The slot.
			/// @param object The object.
			void SetObject(unsigned slot, RendererObject* object);

			/// Releases the object stored in the specified slot.
			/// @param slot The slot.
			void ReleaseObject(unsigned slot);

			void Serialize(IOStream& stream) const;
		};
	}
}