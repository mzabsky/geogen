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
			bool GetValue() const { this->value; }

			virtual MetadataType GetType() const { return MetadataBooleanType; };
		};
	}
}