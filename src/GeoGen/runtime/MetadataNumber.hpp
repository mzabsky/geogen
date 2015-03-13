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

#include "../Number.hpp"
#include "MetadataValue.hpp"

namespace geogen 
{
	namespace runtime
	{
		/// Number node in a metadata tree.
		class MetadataNumber : public MetadataValue
		{
		private:
			Number value;
		public:

			/// Constructor.
			/// @param location The code location.
			/// @param value The value.
			MetadataNumber(CodeLocation location, Number value) : MetadataValue(location), value(value) {};

			/// Gets the value.
			/// @return The value.
			Number GetValue() const { return this->value; }

			virtual MetadataType GetType() const { return METADATA_TYPE_NUMBER; };

			virtual void Serialize(IOStream& stream) const
			{
				stream << this->value;
			}
		};
	}
}