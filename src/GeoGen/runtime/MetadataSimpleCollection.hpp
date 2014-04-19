#pragma once

#include <string>
#include <vector>

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		class MetadataSimpleCollection : public MetadataValue
		{
		private:
			std::vector<MetadataValue*> table;
		public:
			typedef std::vector<MetadataValue const*>::const_iterator const_iterator;

			inline void AddItem(std::string const& name, MetadataValue* value) {				
				this->table.push_back(value);
			};

			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }

			virtual MetadataType GetType() const { return METADATA_TYPE_SIMPLE_COLLECTION; };

			virtual ~MetadataSimpleCollection()
			{
				for(std::vector<MetadataValue*>::iterator it = this->table.begin(); it != this->table.end(); it++)
				{
					delete *it;
				}
			}
		};
	}
}