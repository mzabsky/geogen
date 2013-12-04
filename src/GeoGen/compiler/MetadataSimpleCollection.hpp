#pragma once

#include <string>
#include <vector>

#include "CompilerException.hpp"
#include "MetadataValue.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class MetadataSimpleCollection : MetadataValue
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

			virtual MetadataType GetType() const { return MetadataSimpleCollectionType; };

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