#pragma once

#include <string>

#include "../Number.hpp"
#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		class MetadataNumber : public MetadataValue
		{
		private:
			Number value;
		public:
			MetadataNumber(Number value) : value(value) {};

			Number GetValue() const { return this->value; }

			virtual MetadataType GetType() const { return METADATA_TYPE_NUMBER; };
		};
	}
}