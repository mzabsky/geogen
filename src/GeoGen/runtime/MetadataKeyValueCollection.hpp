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

#include "MetadataValue.hpp"
#include "../utils/OwningMap.hpp"

namespace geogen 
{
	namespace runtime
	{
		/// KeyValueCollection node in a metadata tree.
		class MetadataKeyValueCollection : public MetadataValue, public utils::OwningMap<MetadataValue>
		{
		private:
		public:
			typedef std::map<String, MetadataValue const*>::const_iterator const_iterator;
			//typedef std::map<String, MetadataValue*>::iterator iterator;

			MetadataKeyValueCollection(CodeLocation location) : MetadataValue(location) {}

			void MoveKeyValuesFrom(MetadataKeyValueCollection& another);

			virtual MetadataType GetType() const { return METADATA_TYPE_KEYVALUE_COLLECTION; };
			
			virtual void Serialize(IOStream& stream) const
			{
				//stream << GG_STR("{") << std::endl;
				for (const_iterator it = this->Begin(); it != this->End(); it++)
				{
					stream << it->first << GG_STR(": ");
					it->second->SerializeWithTabs(stream, 1); 
					stream << std::endl;
				}
				//stream << GG_STR("}");
			}
		};
	}
}