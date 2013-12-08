#pragma once

#include <string>
#include <map>

#include "CompilerException.hpp"
#include "MetadataValue.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class MetadataKeyValueCollection : public MetadataValue
		{
		private:
			std::map<std::string, MetadataValue*> table;
		public:
			typedef std::map<std::string, MetadataValue const*>::const_iterator const_iterator;

			inline void AddItem(std::string const& name, MetadataValue* value) {				
				if(this->table.find(name) != this->table.end()){
					throw CompilerException("Metadata entry " + name + " already defined.");
				}

				this->table[name] = value;
			};

			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }

			virtual MetadataType GetType() const { return MetadataKeyValueCollectionType; };

			virtual ~MetadataKeyValueCollection()
			{
				for(std::map<std::string, MetadataValue*>::iterator it = this->table.begin(); it != this->table.end(); it++)
				{
					delete it->second;
				}
			}
		};
	}
}