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
			MetadataIdentifier(std::string value) : value(value) {};

			std::string GetValue() const { this->value; }

			virtual MetadataType GetType() const { return MetadataIdentifierType; };
		};
	}
}