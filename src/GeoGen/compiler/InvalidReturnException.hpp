#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1304_InvalidReturn occurs.
		class InvalidReturnException : public CompilerException
		{
		private:
		public:

			/// Constructor.
			/// @param location The location.
			explicit InvalidReturnException(CodeLocation location) :
				CompilerException(GGE1304_InvalidReturn, location)
			{
			};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Encountered return statement outside of a function on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}