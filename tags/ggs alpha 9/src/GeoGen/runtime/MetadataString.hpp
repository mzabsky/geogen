#pragma once

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		/// String node in a metadata tree.
		class MetadataString : public MetadataValue
		{
		private:
			String value;
		public:

			/// Constructor.
			/// @param location The code location.
			/// @param value The value.
			MetadataString(CodeLocation location, String value) : MetadataValue(location), value(value) {};

			/// Gets the value.
			/// @return The value.
			String GetValue() const { return this->value; }

			virtual MetadataType GetType() const { return METADATA_TYPE_STRING; };

			virtual void Serialize(IOStream& stream) const
			{
				stream << GG_STR("\"") << this->value << GG_STR("\"");
			}
		};
	}
}