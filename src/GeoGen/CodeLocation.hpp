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

#include "Serializable.hpp"

namespace geogen
{
	/// A position in code.
	class CodeLocation : public Serializable
	{
	private:
		int line, column;
	public:

		/// Constructor.
		/// @param line The line.
		/// @param column The column.
		CodeLocation(int line, int column) : line(line), column(column)
		{
		}

		/// Gets the line.
		/// @return The line.
		inline int GetLine() { return this->line; }

		/// Gets the column.
		/// @return The column.
		inline int GetColumn() { return this->column; }

		virtual void Serialize(IOStream& stream) const { stream << "Line " << line << ", column " << column << std::endl; }
	};
}