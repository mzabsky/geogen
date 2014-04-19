#pragma once

#include <string>

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime 
	{
		class MetadataBoolean : public MetadataValue
		{
		private:
			bool value;
		public:
			MetadataBoolean(bool value) : value(value) {};

			bool GetValue() const { return this->value; }

			virtual MetadataType GetType() const { return METADATA_TYPE_BOOL; };
		};
	}
}