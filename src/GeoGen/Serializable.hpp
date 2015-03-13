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

#include "String.hpp"

namespace geogen 
{
	/// Base class for objects that can be serialized into a human readable string representation.
	class Serializable
	{
	private:
	public:

		/// Writes the human readable string representation of this instance to the given stream.
		/// @param stream The stream.
		virtual void Serialize(IOStream& stream) const = 0;

		/// Serialize with tabs.
		/// @param stream The stream.
		/// @param	tabs The tabs.
		virtual void SerializeWithTabs(IOStream& stream, unsigned tabs) const;

		/// Convert this instance into a human readable string representation.
		/// @return A string that represents this instance.
		String ToString() const;
	};
}