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