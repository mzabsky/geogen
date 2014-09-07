#pragma once

#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1201_UnexpectedToken occurs.
		class UnexpectedTokenException : public CompilerException
		{
		private:
			String expectedTokenName;
			/*String actualTokenName;
			String actualTokenText;*/
		public:

			/// Constructor.
			/// @param code The code.
			/// @param location The location.
			/// @param expectedTokenName Name of the expected token.
			explicit UnexpectedTokenException(ErrorCode code, CodeLocation location, String const& expectedTokenName/*, String const& actualTokenName, String const& actualTokenText*/) :
				CompilerException(code, location), expectedTokenName(expectedTokenName)/*, actualTokenName(actualTokenName), actualTokenText(actualTokenText)*/
			{				
			};

			/// Gets expected token name.
			/// @return The expected token name.
			inline String GetExpectedTokenName()
			{
				return expectedTokenName;
			}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Unexpected token on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ", expected \"" << expectedTokenName << "\".";
				return ss.str();
			}
		};
	}
}