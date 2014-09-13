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
			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }

			/// Gets the end iterator.
			/// @return An iterator.
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }

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