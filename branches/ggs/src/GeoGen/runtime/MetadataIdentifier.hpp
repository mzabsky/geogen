#pragma once

#include <string>

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		class MetadataIdentifier : public MetadataValue
		{
		private:
			std::string value;
		public:
			MetadataIdentifier(CodeLocation location, std::string value) : MetadataValue(location), value(value) {};

			std::string GetValue() const { return this->value; }

			virtual MetadataType GetType() const { return METADATA_TYPE_IDENTIFIER; };
		};
	}
}