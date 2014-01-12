#pragma once

#include <string>

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		class MetadataString : public MetadataValue
		{
		private:
			std::string value;
		public:
			MetadataString(std::string value) : value(value) {};

			std::string GetValue() const { this->value; }

			virtual MetadataType GetType() const { return MetadataStringType; };
		};
	}
}