#pragma once

#include <string>

#include "../Number.hpp"
#include "CompilerException.hpp"
#include "MetadataValue.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class MetadataNumber : public MetadataValue
		{
		private:
			geogen::Number value;
		public:
			Number GetValue() const { this->value; }

			virtual MetadataType GetType() const { return MetadataNumberType; };
		};
	}
}