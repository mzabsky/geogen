#pragma once

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		class MetadataString : public MetadataValue
		{
		private:
			String value;
		public:
			MetadataString(CodeLocation location, String value) : MetadataValue(location), value(value) {};

			String GetValue() const { return this->value; }

			virtual MetadataType GetType() const { return METADATA_TYPE_STRING; };

			virtual void Serialize(IOStream& stream) const
			{
				stream << GG_STR("\"") << this->value << GG_STR("\"");
			}
		};
	}
}