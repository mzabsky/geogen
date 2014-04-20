#pragma once

#include <string>
#include <map>

#include "../CodeLocation.hpp"

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
			CodeLocation location;

			// Non-copyable
			MetadataValue(MetadataValue const&) : location(0, 0) {}
			MetadataValue& operator=(MetadataValue const&) {}
		public:
			virtual ~MetadataValue() {};
			MetadataValue(CodeLocation location) : location(location) {};

			CodeLocation GetLocation() const { return this->location; }

			virtual MetadataType GetType() const = 0;
		};
	}
}