#pragma once

#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime 
	{
		/// Boolean node in a metadata tree.
		class MetadataBoolean : public MetadataValue
		{
		private:
			bool value;
		public:

			/// Constructor.
			/// @param location The code location.
			/// @param value Value of the node.
			MetadataBoolean(CodeLocation location, bool value) : MetadataValue(location), value(value) {};

			/// Gets the value.
			/// @return Value of the node.
			bool GetValue() const { return this->value; }

			virtual MetadataType GetType() const { return METADATA_TYPE_BOOLEAN; };

			virtual void Serialize(IOStream& stream) const
			{
				stream << (this->value ? GG_STR("true") : GG_STR("false"));
			}
		};
	}
}