#pragma once

#include <vector>

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		class MetadataList : public MetadataValue
		{
		private:
			std::vector<MetadataValue*> table;
		public:
			typedef std::vector<MetadataValue const*>::const_iterator const_iterator;

			MetadataList(CodeLocation location) : MetadataValue(location) {}

			inline void AddItem(MetadataValue* value) {				
				this->table.push_back(value);
			};

			inline unsigned Size() const { return this->table.size(); }
			inline const_iterator Begin() const { return *(const_iterator*)(&this->table.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->table.end()); }

			virtual MetadataType GetType() const { return METADATA_LIST; };

			virtual ~MetadataList()
			{
				for(std::vector<MetadataValue*>::iterator it = this->table.begin(); it != this->table.end(); it++)
				{
					delete *it;
				}
			}

			virtual void Serialize(IOStream& stream) const
			{
				//stream << GG_STR("{") << std::endl;
				for (const_iterator it = this->Begin(); it != this->End(); it++)
				{
					(*it)->Serialize(stream); stream << std::endl;
				}
				//stream << GG_STR("}");
			}
		};
	}
}