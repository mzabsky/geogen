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

#pragma once

#include <map>

#include "../String.hpp"
#include "../CodeLocation.hpp"

namespace geogen 
{
	namespace runtime
	{		
		/// Types of metadata tree nodes.
		enum MetadataType
		{
			/// Number node. 
			METADATA_TYPE_NUMBER,
			
			/// String node. 
			METADATA_TYPE_STRING,

			/// Boolean node. 
			METADATA_TYPE_BOOLEAN,

			/// Identifier node. 
			METADATA_TYPE_IDENTIFIER,
			
			/// List node. 
			METADATA_LIST,

			/// KeyValueCollection node. 
			METADATA_TYPE_KEYVALUE_COLLECTION
		};

		/// Base class for metadata tree node classes.
		class MetadataValue : public Serializable
		{
		private:
			CodeLocation location;

			// Non-copyable
			MetadataValue(MetadataValue const&) : location(0, 0) {}
			MetadataValue& operator=(MetadataValue const&) {}
		protected:

			/// Base constructor.
			/// @param location The location of declaration.
			MetadataValue(CodeLocation location) : location(location) {};
		public:
			/// Virtual destructor.
			virtual ~MetadataValue() {};			

			/// Gets the location where the node was declared.
			/// @return The location.
			CodeLocation GetLocation() const { return this->location; }

			/// Gets the node type.
			/// @return The type.
			virtual MetadataType GetType() const = 0;

			virtual void Serialize(IOStream& stream) const = 0;
		};

		/// Converts metadata type to string.
		/// @param metadataType Type of the metadata node.
		/// @return A type name.
		String MetadataTypeToString(MetadataType metadataType);
	}
}