#pragma once

#include <vector>

namespace geogen
{
	namespace renderer
	{
		class RendererObject;

		/// A table of RendererObject items stored in a fixed number of slots. Owns the contained
		/// pointers.
		class RendererObjectTable
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

			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }
			inline iterator Begin() { return this->table.begin(); }
			inline iterator End() { return this->table.end(); }

			/// Gets an object from the specified slot.
			/// @param slot The slot.
			/// @return The object stored in the specified slot.
			RendererObject* GetObject(unsigned slot);

			/// Stores the object into the specified slot.
			/// @param slot			   The slot.
			/// @param [in,out] object The object.
			void SetObject(unsigned slot, RendererObject* object);

			/// Releases the object stored in the specified slot.
			/// @param slot The slot.
			void ReleaseObject(unsigned slot);
		};
	}
}