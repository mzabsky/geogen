#pragma once

#include <string>
#include <map>

namespace geogen 
{
	namespace runtime
	{
		enum MetadataType
		{
			METADATA_TYPE_NUMBER,
			METADATA_TYPE_STRING,
			METADATA_TYPE_BOOL,
			METADATA_TYPE_IDENTIFIER,
			METADATA_TYPE_SIMPLE_COLLECTION,
			METADATA_TYPE_KEYVALUE_COLLECTION
		};

		class MetadataValue
		{
		private:
			// Non-copyable
			MetadataValue(MetadataValue const&) {}
			MetadataValue& operator=(MetadataValue const&) {}
		public:
			virtual ~MetadataValue() {};

			MetadataValue() {};

			virtual MetadataType GetType() const = 0;
		};
	}
}