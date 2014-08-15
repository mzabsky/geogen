#pragma once

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
			MetadataBoolean(CodeLocation location, bool value) : MetadataValue(location), value(value) {};

			bool GetValue() const { return this->value; }

			virtual MetadataType GetType() const { return METADATA_TYPE_BOOLEAN; };

			virtual void Serialize(IOStream& stream) const
			{
				stream << (this->value ? GG_STR("true") : GG_STR("false"));
			}
		};
	}
}