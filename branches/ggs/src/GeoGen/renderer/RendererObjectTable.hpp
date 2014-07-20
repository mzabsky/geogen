#pragma once

#include <vector>

namespace geogen
{
	namespace renderer
	{
		class RendererObject;

		/// <summary> A table of <see cref="RendererObject"/>s stored in a fixed number of slots. </summary>
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

			RendererObjectTable(unsigned size) : table(std::vector<RendererObject*>(size, NULL)) {}
			~RendererObjectTable();

			inline unsigned GetSize() { return this->table.size(); }

			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }
			inline iterator Begin() { return this->table.begin(); }
			inline iterator End() { return this->table.end(); }

			RendererObject* GetObject(unsigned slot);
			void SetObject(unsigned slot, RendererObject* object);
			void ReleaseObject(unsigned slot);
		};
	}
}