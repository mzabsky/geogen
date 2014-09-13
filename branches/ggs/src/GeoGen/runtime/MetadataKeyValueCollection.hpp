#pragma once

#include <map>

#include "MetadataValue.hpp"
#include "../utils/OwningMap.hpp"

namespace geogen 
{
	namespace runtime
	{
		/// KeyValueCollection node in a metadata tree.
		class MetadataKeyValueCollection : public MetadataValue, public utils::OwningMap<MetadataValue>
		{
		private:
		public:
			typedef std::map<String, MetadataValue const*>::const_iterator const_iterator;
			//typedef std::map<String, MetadataValue*>::iterator iterator;

			MetadataKeyValueCollection(CodeLocation location) : MetadataValue(location) {}

			void MoveKeyValuesFrom(MetadataKeyValueCollection& another);

			virtual MetadataType GetType() const { return METADATA_TYPE_KEYVALUE_COLLECTION; };
			
			virtual void Serialize(IOStream& stream) const
			{
				//stream << GG_STR("{") << std::endl;
				for (const_iterator it = this->Begin(); it != this->End(); it++)
				{
					stream << it->first << GG_STR(": ");
					it->second->SerializeWithTabs(stream, 1); 
					stream << std::endl;
				}
				//stream << GG_STR("}");
			}
		};
	}
}