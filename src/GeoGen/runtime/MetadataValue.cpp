/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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
	case METADATA_LIST:
		return GG_STR("List");
	case METADATA_TYPE_KEYVALUE_COLLECTION: 
		return GG_STR("KeyValueCollection");
	default: 
		throw InternalErrorException(GG_STR("Invalid metadata type."));
	}
}