#pragma once

#include <string>

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class MetadataString : public MetadataValue
		{
		private:
			std::string value;
		public:
			std::string GetValue() const { this->value; }

			virtual MetadataType GetType() const { return MetadataStringType; };
		};
	}
}