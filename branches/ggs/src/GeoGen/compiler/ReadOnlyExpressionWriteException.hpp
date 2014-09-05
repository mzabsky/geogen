#pragma once

#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class ReadOnlyExpressionWriteException : public CompilerException
		{
		private:			
		public:
			explicit ReadOnlyExpressionWriteException(CodeLocation location) :
				CompilerException(GGE1307_ReadOnlyExpressionWrite, location)
			{
			};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to write read-only expression on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}