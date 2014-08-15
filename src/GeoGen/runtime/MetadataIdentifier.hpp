#pragma once

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		class MetadataIdentifier : public MetadataValue
		{
		private:
			String value;
		public:
			MetadataIdentifier(CodeLocation location, String value) : MetadataValue(location), value(value) {};

			String GetValue() const { return this->value; }

			virtual MetadataType GetType() const { return METADATA_TYPE_IDENTIFIER; };

			virtual void Serialize(IOStream& stream) const
			{
				stream << this->value;
			}
		};
	}
}