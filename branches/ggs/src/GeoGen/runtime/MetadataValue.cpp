#include "MetadataValue.hpp"
#include "..\InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

string geogen::runtime::MetadataTypeToString(MetadataType metadataType)
{
	switch (metadataType)
	{
	case METADATA_TYPE_NUMBER: 
		return "Number";
	case METADATA_TYPE_STRING: 
		return "String";
	case METADATA_TYPE_BOOLEAN:
		return "Boolean";
	case METADATA_TYPE_IDENTIFIER:
		return "Identifier";
	case METADATA_TYPE_SIMPLE_COLLECTION:
		return "SimpleCollection";
	case METADATA_TYPE_KEYVALUE_COLLECTION: 
		return "KeyValueCollection";
	default: 
		throw InternalErrorException("Invalid metadata type.");
	}
}