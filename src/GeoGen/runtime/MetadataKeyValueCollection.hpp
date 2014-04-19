#pragma once

#include <string>
#include <map>

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		class MetadataKeyValueCollection : public MetadataValue
		{
		private:
			std::map<std::string, MetadataValue*> table;
		public:
			typedef std::map<std::string, MetadataValue const*>::const_iterator const_iterator;
			typedef std::map<std::string, MetadataValue*>::iterator iterator;

			bool AddItem(std::string const& name, MetadataValue* value);

			inline unsigned Size() const { return this->table.size(); }
			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }
			inline iterator Begin() { return this->table.begin(); }
			inline iterator End() { return this->table.end(); }

			void MoveKeyValuesFrom(MetadataKeyValueCollection& another);

			inline const_iterator FindItem(std::string const& key) const { return *(const_iterator*)(&this->table.find(key)); }
			inline iterator FindItem(std::string const& key) { return this->table.find(key); }
			inline bool ContainsItem(std::string const& key) const { return this->FindItem(key) != this->End(); };
			MetadataValue const* GetItem(std::string const& key) const;
			MetadataValue* GetItem(std::string const& key);

			virtual MetadataType GetType() const { return METADATA_TYPE_KEYVALUE_COLLECTION; };

			virtual ~MetadataKeyValueCollection();
		};
	}
}