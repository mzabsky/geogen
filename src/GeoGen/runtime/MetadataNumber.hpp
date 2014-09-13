#pragma once

#include "../Number.hpp"
#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		/// Number node in a metadata tree.
		class MetadataNumber : public MetadataValue
		{
		private:
			Number value;
		public:

			/// Constructor.
			/// @param location The code location.
			/// @param value The value.
			MetadataNumber(CodeLocation location, Number value) : MetadataValue(location), value(value) {};

			/// Gets the value.
			/// @return The value.
			Number GetValue() const { return this->value; }

			virtual MetadataType GetType() const { return METADATA_TYPE_NUMBER; };

			virtual void Serialize(IOStream& stream) const
			{
				stream << this->value;
			}
		};
	}
}