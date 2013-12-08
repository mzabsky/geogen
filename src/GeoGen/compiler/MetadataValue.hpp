#pragma once

#include <string>
#include <map>

namespace geogen 
{
	namespace compiler 
	{
		enum MetadataType
		{
			MetadataNumberType,
			MetadataStringType,
			MetadataBooleanType,
			MetadataIdentifierType,
			MetadataSimpleCollectionType,
			MetadataKeyValueCollectionType
		};

		class MetadataValue
		{
		private:
			// Non-copyable
			MetadataValue(MetadataValue const&) {}
			MetadataValue& operator=(MetadataValue const&) {}
		public:
			MetadataValue() {};

			virtual MetadataType GetType() const = 0;
		};
	}
}