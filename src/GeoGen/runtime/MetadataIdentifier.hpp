#pragma once

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		/// Identifier node in a metadata tree.
		class MetadataIdentifier : public MetadataValue
		{
		private:
			String value;
		public:

			/// Constructor.
			/// @param location The code location.
			/// @param value The value.
			MetadataIdentifier(CodeLocation location, String value) : MetadataValue(location), value(value) {};

			/// Gets the value.
			/// @return The value.
			String GetValue() const { return this->value; }

			virtual MetadataType GetType() const { return METADATA_TYPE_IDENTIFIER; };

			virtual void Serialize(IOStream& stream) const
			{
				stream << this->value;
			}
		};
	}
}