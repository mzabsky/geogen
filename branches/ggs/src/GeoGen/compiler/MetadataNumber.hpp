#pragma once

#include <string>

#include "../Number.hpp"
#include "MetadataValue.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class MetadataNumber : public MetadataValue
		{
		private:
			Number value;
		public:
			MetadataNumber(Number value) : value(value) {};

			Number GetValue() const { this->value; }

			virtual MetadataType GetType() const { return MetadataNumberType; };
		};
	}
}