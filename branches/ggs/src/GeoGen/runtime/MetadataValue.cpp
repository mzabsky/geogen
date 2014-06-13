#include "MetadataValue.hpp"
#include "..\InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

String geogen::runtime::MetadataTypeToString(MetadataType metadataType)
{
	switch (metadataType)
	{
	case METADATA_TYPE_NUMBER: 
		return GG_STR("Number");
	case METADATA_TYPE_STRING: 
		return GG_STR("String");
	case METADATA_TYPE_BOOLEAN:
		return GG_STR("Boolean");
	case METADATA_TYPE_IDENTIFIER:
		return GG_STR("Identifier");
	case METADATA_TYPE_SIMPLE_COLLECTION:
		return GG_STR("SimpleCollection");
	case METADATA_TYPE_KEYVALUE_COLLECTION: 
		return GG_STR("KeyValueCollection");
	default: 
		throw InternalErrorException(GG_STR("Invalid metadata type."));
	}
}