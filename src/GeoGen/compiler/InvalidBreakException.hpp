#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1301_InvalidBreak occurs.
		class InvalidBreakException : public CompilerException
		{
		private:
		public:

			/// Constructor.
			/// @param location The location.
			explicit InvalidBreakException(CodeLocation location) :
				CompilerException(GGE1301_InvalidBreak, location)
			{
			};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Encountered break statement outside of a loop on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}