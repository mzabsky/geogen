#pragma once

#include "Serializable.hpp"

namespace geogen
{
	class CodeLocation : public Serializable
	{
	private:
		int line, column;
	public:
		CodeLocation(int line, int column) : line(line), column(column)
		{
		}

		inline int GetLine() { return this->line; }

		inline int GetColumn() { return this->column; }

		virtual void Serialize(IOStream& stream) const { stream << "Line " << line << ", column " << column << std::endl; }
	};
}