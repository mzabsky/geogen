#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1303_InvalidContinue occurs.
		class InvalidContinueException : public CompilerException
		{
		private:
		public:

			/// Constructor.
			/// @param location The location.
			explicit InvalidContinueException(CodeLocation location) :
				CompilerException(GGE1303_InvalidContinue, location)
			{
			};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Encountered continue statement outside of a loop on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}